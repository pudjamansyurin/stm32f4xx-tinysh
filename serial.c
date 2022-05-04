/*
 * serial.c
 *
 *  Created on: May 5, 2022
 *      Author: pujak
 */
#include "serial.h"

/* Public function definitions */
void tinysh_chars_in(unsigned char *str, uint16_t size)
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

