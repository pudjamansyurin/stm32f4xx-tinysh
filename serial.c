/*
 * serial.c
 *
 *  Created on: May 5, 2022
 *      Author: pujak
 */
#include "serial.h"

/* Private variables */
static UART_HandleTypeDef *huart;

/* Private function definitions */
static void transmit(char *str, int len)
{
  HAL_UART_Transmit(huart, (uint8_t*) str, len, HAL_MAX_DELAY);
}

/* Public function definitions */
void stdout_init(UART_HandleTypeDef *uart)
{
  huart = uart;

  /* disable stdio buffering */
  setvbuf(stdout, NULL, _IONBF, 0);
}

void stdin_chars_in(unsigned char *str, uint16_t size)
{
  while (size--)
    tinysh_char_in(*str++);
}

/* we must provide this function to use tinysh  */
void tinysh_puts(char *s)
{
//  while (*s)
//    putchar(*s++);
  puts(s);
}

void tinysh_char_out(unsigned char c)
{
  putchar((int) c);
}

/* Replace weak syscalls routines */
int __io_putchar(int ch)
{
  transmit((char*) &ch, 1);
  return (ch);
}

int _write(int file, char *ptr, int len)
{
  transmit(ptr, len);
  return len;
}
