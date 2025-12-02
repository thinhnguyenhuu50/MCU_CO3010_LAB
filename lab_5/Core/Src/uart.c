/*
 * uart.c
 *
 *  Created on: 2 thg 12, 2025
 *      Author: thinh
 */

#include "uart.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART2) {
		HAL_UART_Transmit(&huart2, &temp, 1, 50);
		HAL_UART_Receive_IT(&huart2, &temp, 1);
	}
}
