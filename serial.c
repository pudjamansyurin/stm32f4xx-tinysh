/*
 * serial.c
 *
 *  Created on: Apr 19, 2022
 *      Author: pudja
 */

#include "serial.h"
#include "circular.h"

/* Private variables */
static circular_t hcirc;

/* Public function definitions */
void serial_init(UART_HandleTypeDef *uart, uint8_t *buffer, uint16_t size)
{
  circular_init(&hcirc, uart, buffer, size);

  /* disable stdio buffering */
  setvbuf(stdout, NULL, _IONBF, 0);
}

void serial_set_callback(void (*cb)(uint8_t *pbuffer, uint16_t size))
{
  circular_set_callback(&hcirc, cb);
}

HAL_StatusTypeDef serial_start(void)
{
  return circular_start(&hcirc);
}

void serial_irq_dma(void)
{
  circular_irq_dma(&hcirc);
}

void serial_irq_uart(void)
{
  circular_irq_uart(&hcirc);
}

void serial_line_in(const uint8_t *data, uint16_t size)
{
  unsigned char ch;

  for (uint16_t i = 0; i < size; i++)
  {
    ch = data[i];
    tinysh_char_in(ch);
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
  HAL_UART_Transmit(hcirc.huart, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
  return (ch);
}
