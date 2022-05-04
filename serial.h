/*
 * serial.h
 *
 *  Created on: May 5, 2022
 *      Author: pujak
 */

#ifndef STM32F4XX_TINYSH_SERIAL_H_
#define STM32F4XX_TINYSH_SERIAL_H_

#include "stm32f4xx-stdin/stdin.h"
#include "tinysh.h"
#include "stdio.h"

/* Public function declarations */
void stdout_init(UART_HandleTypeDef *uart);
void stdin_chars_in(unsigned char *str, uint16_t size);

#endif /* STM32F4XX_TINYSH_SERIAL_H_ */
