/*
 * serial.c
 *
 *  Created on: Apr 19, 2022
 *      Author: pudja
 */

#include "serial.h"
#include <stdio.h>

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

/* Public function definitions */
void serial_init(UART_HandleTypeDef *uart, uint8_t *buffer, uint16_t size)
{
  hserial.huart = uart;
  hserial.buffer = buffer;
  hserial.size = size;

  /* disable stdio buffering */
  setvbuf(stdout, NULL, _IONBF, 0);
}

void serial_read(void)
{
  HAL_UARTEx_ReceiveToIdle_DMA(hserial.huart, hserial.buffer, hserial.size);
  __HAL_DMA_DISABLE_IT(hserial.huart->hdmarx, DMA_IT_HT);
  __HAL_DMA_ENABLE_IT(hserial.huart->hdmarx, DMA_IT_TC);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == hserial.huart->Instance)
  {
    tinysh_line_in(hserial.buffer, Size);
    serial_read();
  }
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
