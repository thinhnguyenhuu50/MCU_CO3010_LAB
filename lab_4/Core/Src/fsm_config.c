/*
 * fsm_config.c
 *
 *  Created on: Nov 14, 2025
 *      Author: thinh
 */

#include "fsm_config.h"
#include "global.h"
#include "gpio.h"
#include "scheduler.h"

static int8_t task1 = -1, task2 = -1, task3 = -1;

// Forward declaration
static void blink_red();
static void blink_amber();
static void blink_green();

void fsm_config() {
	switch (status) {
		case SET_RED:
			if (task1 == -1) {
				task1 = SCH_add(blink_red, 0, 500);
			}
			break;
		case SET_AMBER:
			SCH_delete(task1);
			task1 = -1;

			if (task2 == -1) {
				task2 = SCH_add(blink_amber, 0, 500);
			}
			break;
		case SET_GREEN:
			SCH_delete(task2);
			task2 = -1;

			if (task3 == -1) {
				task3 = SCH_add(blink_green, 0, 500);
			}

		default:
			break;
	}
}

static void blink_red() {
	HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
	HAL_GPIO_TogglePin(RED2_GPIO_Port, RED2_Pin);
}

static void blink_amber() {
	HAL_GPIO_TogglePin(AMBER1_GPIO_Port, AMBER1_Pin);
	HAL_GPIO_TogglePin(AMBER2_GPIO_Port, AMBER2_Pin);
}

static void blink_green() {
	HAL_GPIO_TogglePin(GREEN1_GPIO_Port, GREEN1_Pin);
	HAL_GPIO_TogglePin(GREEN2_GPIO_Port, GREEN2_Pin);
}
