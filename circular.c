/*
 * circular.c
 *
 *  Created on: Apr 19, 2022
 *      Author: pudja
 */
#include "circular.h"
#include <string.h>

#define HDMA(_X_) ((_X_)->huart->hdmarx)

/* Private function prototypes
 * --------------------------------------------*/
static void check_buffer(circular_t *c);
static void fill_buffer(circular_t *c, uint16_t pos, uint16_t size);

/* Public function implementations
 * --------------------------------------------*/
void circular_init(circular_t *c, UART_HandleTypeDef *uart, uint8_t *buffer, uint16_t size)
{
  c->huart = uart;
  c->buffer = buffer;
  c->size = size;

  circular_flush(c);
}

void circular_set_callback(circular_t *c, void (*cb)(uint8_t *buffer, uint16_t size))
{
  c->callback = cb;
}

void circular_flush(circular_t *c)
{
  memset(c->buffer, 0, c->size);
  c->pos = 0;
}

HAL_StatusTypeDef circular_start(circular_t *c)
{
  HAL_StatusTypeDef status;

  /* Enable interrupts */
  __HAL_UART_ENABLE_IT(c->huart, UART_IT_IDLE);
  __HAL_DMA_ENABLE_IT(HDMA(c), DMA_IT_TC);
  __HAL_DMA_ENABLE_IT(HDMA(c), DMA_IT_HT);

  /* Start receiving UART in DMA mode */
  status = HAL_UART_Receive_DMA(c->huart, c->buffer, c->size);
  return (status);
}

HAL_StatusTypeDef circular_stop(circular_t *c)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_DMAStop(c->huart);
  return (status);
}

void circular_irq_dma(circular_t *c)
{
  /* Handle HT interrupt */
  if (__HAL_DMA_GET_IT_SOURCE(HDMA(c), DMA_IT_HT))
  {
    __HAL_DMA_CLEAR_FLAG(HDMA(c), __HAL_DMA_GET_HT_FLAG_INDEX(HDMA(c)));
    check_buffer(c);
  }

  /* Handle TC interrupt */
  else if (__HAL_DMA_GET_IT_SOURCE(HDMA(c), DMA_IT_TC))
  {
    __HAL_DMA_CLEAR_FLAG(HDMA(c), __HAL_DMA_GET_TC_FLAG_INDEX(HDMA(c)));
    check_buffer(c);
  }

  /* Handle ERROR interrupt */
  else
  {
    __HAL_DMA_CLEAR_FLAG(HDMA(c), __HAL_DMA_GET_TE_FLAG_INDEX(HDMA(c)));
    __HAL_DMA_CLEAR_FLAG(HDMA(c), __HAL_DMA_GET_FE_FLAG_INDEX(HDMA(c)));
    __HAL_DMA_CLEAR_FLAG(HDMA(c), __HAL_DMA_GET_DME_FLAG_INDEX(HDMA(c)));

    HAL_UART_Receive_DMA(c->huart, c->buffer, c->size);
  }
}

void circular_irq_uart(circular_t *c)
{
  if (__HAL_UART_GET_FLAG(c->huart, UART_FLAG_IDLE))
  {
    __HAL_UART_CLEAR_IDLEFLAG(c->huart);
    check_buffer(c);
  }
}

/* Private function implementations
 * --------------------------------------------*/
static void check_buffer(circular_t *c)
{
  uint16_t pos, new;

  /* Calculate current position in buffer */
  new = __HAL_DMA_GET_COUNTER(HDMA(c));
  pos = c->size - new;

  /* Check change in received data */
  if (pos != c->pos)
  {
    if (pos > c->pos)
      /* Current position is over previous one */
      /* We are in "linear" mode */
      /* Process data directly by subtracting "pointers" */
      fill_buffer(c, c->pos, pos - c->pos);
    else
    {
      /* We are in "overflow" mode */
      /* First process data to the end of buffer */
      fill_buffer(c, c->pos, c->size - c->pos);
      /* Check and continue with beginning of buffer */
      if (pos > 0)
        fill_buffer(c, 0, pos);
    }
  }

  /* Check and manually update if we reached end of buffer */
  c->pos = (pos == c->size) ? 0 : pos;
}

static void fill_buffer(circular_t *c, uint16_t pos, uint16_t size)
{
  if (NULL == c->callback)
    return;

  c->callback(&c->buffer[pos], size);
}

