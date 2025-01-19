/*
 * uart.h
 *
 *  Created on: May 20, 2024
 *      Author: danny
 */

#ifndef INC_UART_H_
#define INC_UART_H_
#include "stm32f4xx.h"
HAL_StatusTypeDef uartinit();
HAL_StatusTypeDef UART_SendString(char *str);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle);
#endif /* INC_UART_H_ */
