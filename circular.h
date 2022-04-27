/*
 * circular.h
 *
 *  Created on: Apr 19, 2022
 *      Author: pudja
 */

#ifndef SRC_CIRCULAR_CIRCULAR_H_
#define SRC_CIRCULAR_CIRCULAR_H_

#include "stm32f4xx_hal.h"

/* Exported types
 * --------------------------------------------*/
typedef struct
{
  UART_HandleTypeDef *huart;
  uint8_t *buffer;
  uint16_t size;
  uint16_t pos;
  void (*callback)(uint8_t *buffer, uint16_t size);
} circular_t;

/* Public function prototypes
 * --------------------------------------------*/
void circular_init(circular_t *c, UART_HandleTypeDef *uart, uint8_t *buffer, uint16_t size);
void circular_set_callback(circular_t *c, void (*cb)(uint8_t *buffer, uint16_t size));
void circular_flush(circular_t *c);
HAL_StatusTypeDef circular_start(circular_t *c);
HAL_StatusTypeDef circular_stop(circular_t *c);
void circular_irq_dma(circular_t *c);
void circular_irq_uart(circular_t *c);

#endif /* SRC_CIRCULAR_CIRCULAR_H_ */
