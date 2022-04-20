/*
 * serial.h
 *
 *  Created on: Apr 19, 2022
 *      Author: pudja
 */

#ifndef SRC_SERIAL_SERIAL_H_
#define SRC_SERIAL_SERIAL_H_

#include "stm32f4xx_hal.h"
#include "tinysh.h"
#include <stdio.h>

typedef struct {
  UART_HandleTypeDef *huart;
  uint8_t *buffer;
  uint16_t size;
} serialsh_t;

/* Public function declarations */
void serial_init(UART_HandleTypeDef *puart, uint8_t *buffer, uint16_t size);
void serial_read(void);

#endif /* SRC_SERIAL_SERIAL_H_ */
