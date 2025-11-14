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
#include "button.h"

static int8_t task_ID_red = -1, task_ID_amber = -1, task_ID_green = -1;

// Forward declaration
static void blink_red();
static void blink_amber();
static void blink_green();

void fsm_config() {
	switch (status) {
	case SET_RED:
		if (task_ID_red == -1) {
			task_ID_red = SCH_add(blink_red, 0, 500);
		}

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			light_disable();
			SCH_delete(task_ID_red);
			task_ID_red = -1;
			status = SET_AMBER;
		}
		break;
	case SET_AMBER:
		if (task_ID_amber == -1) {
			task_ID_amber = SCH_add(blink_amber, 0, 500);
		}

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			light_disable();
			SCH_delete(task_ID_amber);
			task_ID_amber = -1;
			status = SET_GREEN;
		}
		break;
	case SET_GREEN:
		if (task_ID_green == -1) {
			task_ID_green = SCH_add(blink_green, 0, 500);
		}

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			light_disable();
			SCH_delete(task_ID_green);
			task_ID_green = -1;
			status = INIT;
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
