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

/* Public function declarations */
void serial_init(UART_HandleTypeDef *puart, uint8_t *buffer, uint16_t size);
void serial_set_callback(void (*cb)(uint8_t *pbuffer, uint16_t size));
HAL_StatusTypeDef serial_start(void);
void serial_irq_dma(void);
void serial_irq_uart(void);

void serial_line_in(const uint8_t *data, uint16_t size);

#endif /* SRC_SERIAL_SERIAL_H_ */
