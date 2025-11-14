/*
 * global.c
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#include "global.h"
#include "gpio.h"

#define DEFAULT_RED_TIME  	5
#define DEFAULT_AMBER_TIME  2
#define DEFAULT_GREEN_TIME  3

fsm_status_t status = INIT;

uint8_t green_duration 	= DEFAULT_GREEN_TIME;
uint8_t amber_duration 	= DEFAULT_AMBER_TIME;
uint8_t red_duration 	= DEFAULT_RED_TIME;

void light_disable() {
	HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, 1);
	HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, 1);
	HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, 1);
	HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, 1);
	HAL_GPIO_WritePin(AMBER2_GPIO_Port, AMBER2_Pin, 1);
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, 1);
}
