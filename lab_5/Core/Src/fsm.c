/*
 * uart.c
 *
 *  Created on: 2 thg 12, 2025
 *      Author: thinh
 */

#include "fsm.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "adc.h"
#include "software_timer.h"

#define MAX_BUFFER_SIZE 30
#define CMD_NONE   0
#define CMD_RST    1
#define CMD_OK     2

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

uint8_t command_flag = 0;
uint8_t command_data = CMD_NONE;

typedef enum {
	UART_IDLE = 0, UART_WAIT_OK
} UART_State_t;

UART_State_t uart_state = UART_IDLE;

uint32_t adc_value = 0;
char uart_tx_buffer[32];

/* States for the command parser */
typedef enum {
	CMD_WAIT_START = 0,
	CMD_WAIT_CMD1,
	CMD_WAIT_RST2,
	CMD_WAIT_RST3,
	CMD_WAIT_OK2,
	CMD_WAIT_END
} CommandParserState_t;

static CommandParserState_t state = CMD_WAIT_START;
static uint8_t pending_cmd = CMD_NONE;

void uart_init() {
	HAL_UART_Receive_IT(&huart2, &temp, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART2) {
		buffer[index_buffer++] = temp;
		if (index_buffer == MAX_BUFFER_SIZE)
			index_buffer = 0;

		buffer_flag = 1; // notify main loop that a new byte has arrived

		HAL_UART_Transmit(&huart2, &temp, 1, HAL_MAX_DELAY);
		HAL_UART_Receive_IT(&huart2, &temp, 1);
	}
}

void command_parser_fsm(void) {
	/* Get the latest received character from the ring buffer */
	uint8_t c;
	if (index_buffer == 0) {
		c = buffer[MAX_BUFFER_SIZE - 1];
	} else {
		c = buffer[index_buffer - 1];
	}

	switch (state) {
		case CMD_WAIT_START:
			if (c == '!') {
				state = CMD_WAIT_CMD1;
			}
			break;

		case CMD_WAIT_CMD1:
			if (c == 'R') {
				pending_cmd = CMD_RST;
				state = CMD_WAIT_RST2;
			} else if (c == 'O') {
				pending_cmd = CMD_OK;
				state = CMD_WAIT_OK2;
			} else if (c == '!') {
				// start of a new command
				state = CMD_WAIT_CMD1;
			} else {
				// invalid, reset parser
				state = CMD_WAIT_START;
			}
			break;

			/* --- Parse !RST# --- */
		case CMD_WAIT_RST2:       // already got "!R"
			if (c == 'S') {
				state = CMD_WAIT_RST3;
			} else if (c == '!') {
				state = CMD_WAIT_CMD1;
			} else {
				state = CMD_WAIT_START;
			}
			break;

		case CMD_WAIT_RST3:       // "!RS"
			if (c == 'T') {
				state = CMD_WAIT_END;
			} else if (c == '!') {
				state = CMD_WAIT_CMD1;
			} else {
				state = CMD_WAIT_START;
			}
			break;

			/* --- Parse !OK# --- */
		case CMD_WAIT_OK2:        // "!O"
			if (c == 'K') {
				state = CMD_WAIT_END;
			} else if (c == '!') {
				state = CMD_WAIT_CMD1;
			} else {
				state = CMD_WAIT_START;
			}
			break;

		case CMD_WAIT_END:        // either "!RST" or "!OK"
			if (c == '#') {
				command_data = pending_cmd;
				command_flag = 1;       // notify uart_communiation_fsm()
				pending_cmd = CMD_NONE;
				state = CMD_WAIT_START;
			} else if (c == '!') {
				// maybe start of a new command
				state = CMD_WAIT_CMD1;
			} else {
				state = CMD_WAIT_START;
			}
			break;

		default:
			state = CMD_WAIT_START;
			break;
	}
}

void uart_communiation_fsm(void) {
	switch (uart_state) {
		case UART_IDLE:
			if (command_flag) {
				if (command_data == CMD_RST) {
					command_flag = 0;

					/* Read current ADC value once */
					adc_value = HAL_ADC_GetValue(&hadc1);

					/* Format and send packet: !ADC=xxxx# */
					int len = sprintf(uart_tx_buffer, "!ADC=%lu#\r\n",
							(unsigned long) adc_value);
					HAL_UART_Transmit(&huart2, (uint8_t*) uart_tx_buffer, len, 1000);

					/* Start 3 s software timer and wait for OK */
					timer_set(UART_TIMER_INDEX, UART_TIMEOUT_MS);
					uart_state = UART_WAIT_OK;
				} else {
					/* Ignore unexpected commands in IDLE */
					command_flag = 0;
				}
			}
			break;

		case UART_WAIT_OK:
			if (command_flag) {
				if (command_data == CMD_OK) {
					/* Got ACK, end transaction */
					command_flag = 0;
					uart_state = UART_IDLE;
				} else if (command_data == CMD_RST) {
					/* Optional: treat as a new request, send fresh ADC value */
					command_flag = 0;

					adc_value = HAL_ADC_GetValue(&hadc1);
					int len = sprintf(uart_tx_buffer, "!ADC=%lu#\r\n",
							(unsigned long) adc_value);
					HAL_UART_Transmit(&huart2, (uint8_t*) uart_tx_buffer, len, 1000);

					/* Restart 3 s timer */
					timer_set(UART_TIMER_INDEX, UART_TIMEOUT_MS);
				} else {
					command_flag = 0;
				}
			} else {
				/* No new command, check software timer timeout */
				if (timer_is_expired(UART_TIMER_INDEX)) {
					/* Re-send the SAME adc_value (as required) */
					int len = sprintf(uart_tx_buffer, "!ADC=%lu#\r\n",
							(unsigned long) adc_value);
					HAL_UART_Transmit(&huart2, (uint8_t*) uart_tx_buffer, len, 1000);

					/* Start waiting again for another 3 s */
					timer_set(UART_TIMER_INDEX, UART_TIMEOUT_MS);
				}
			}
			break;

		default:
			uart_state = UART_IDLE;
			break;
	}
}
