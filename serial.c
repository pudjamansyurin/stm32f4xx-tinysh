/*
 * serial.c
 *
 *  Created on: Apr 19, 2022
 *      Author: pudja
 */

#include "serial.h"

/* Private variables */
static serialsh_t hserial;

/* Private function */
static void tinysh_line_in(const uint8_t *data, uint16_t size)
{
  while (size--)
  {
    tinysh_char_in((unsigned char) *data++);
  }
}

static void UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  tinysh_line_in(hserial.buffer, Size);
  serial_read_dma();
}

static void UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  serial_read_dma();
}

/* Public function definitions */
void serial_init(UART_HandleTypeDef *uart, uint8_t *buffer, uint16_t size)
{
  hserial.huart = uart;
  hserial.buffer = buffer;
  hserial.size = size;

  HAL_UART_RegisterCallback(uart, HAL_UART_ERROR_CB_ID, UART_ErrorCallback);
  HAL_UART_RegisterRxEventCallback(uart, UARTEx_RxEventCallback);

  /* disable stdio buffering */
  setvbuf(stdout, NULL, _IONBF, 0);
}

HAL_StatusTypeDef serial_read(void)
{
  HAL_StatusTypeDef status;
  uint16_t RxLen = 0;

  status = HAL_UARTEx_ReceiveToIdle(hserial.huart, hserial.buffer, hserial.size, &RxLen, HAL_MAX_DELAY);
  tinysh_line_in(hserial.buffer, RxLen);

  return (status);
}

HAL_StatusTypeDef serial_read_dma(void)
{
  HAL_StatusTypeDef status;

  status = HAL_UARTEx_ReceiveToIdle_DMA(hserial.huart, hserial.buffer, hserial.size);
  if (status == HAL_OK)
  {
    __HAL_DMA_DISABLE_IT(hserial.huart->hdmarx, DMA_IT_HT);
    __HAL_DMA_ENABLE_IT(hserial.huart->hdmarx, DMA_IT_TC);
  }

  return (status);
}

/* we must provide this function to use tinysh  */
void tinysh_char_out(unsigned char c)
{
  putchar((int) c);
}

/* Replace weak syscalls routines */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(hserial.huart, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
  return (ch);
}
