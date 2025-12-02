/*
 * uart.h
 *
 *  Created on: 2 thg 12, 2025
 *      Author: thinh
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stdint.h"

#define UART_TIMER_INDEX   0     // use a dedicated timer slot
#define UART_TIMEOUT_MS    3000  // 3 seconds

extern uint8_t buffer_flag;

void uart_init();
void command_parser_fsm();
void uart_communiation_fsm();

#endif /* INC_UART_H_ */
