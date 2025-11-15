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

static int8_t task_ID_red = -1, task_ID_amber = -1, task_ID_green = -1, task_ID_increase = -1;

// Forward declaration
static void blink_red();
static void blink_amber();
static void blink_green();
static void button_process(uint8_t *input);

void fsm_config() {
	switch (status) {
	case SET_RED:
		if (task_ID_red == -1) {
			task_ID_red = SCH_add(blink_red, 0, 500);
		}

		button_process(&red_input);

		// Display value and mode number
		set_led7seg_Road1(countUp);
		set_led7seg_Road2(2);

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			SCH_delete(task_ID_red);
			task_ID_red = -1;

			light_disable();

			amber_input = amber_duration;
			countUp = 1;

			status = SET_AMBER;
		}
		break;
	case SET_AMBER:
		if (task_ID_amber == -1) {
			task_ID_amber = SCH_add(blink_amber, 0, 500);
		}

		button_process(&amber_input);

		// Display value and mode number
		set_led7seg_Road1(countUp);
		set_led7seg_Road2(3);

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			SCH_delete(task_ID_amber);
			task_ID_amber = -1;

			light_disable();

			green_input = green_duration;
			countUp = 1;

			status = SET_GREEN;
		}

		break;
	case SET_GREEN:
		if (task_ID_green == -1) {
			task_ID_green = SCH_add(blink_green, 0, 500);
		}

		button_process(&green_input);

		// Display value and mode number
		set_led7seg_Road1(countUp);
		set_led7seg_Road2(4);

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			SCH_delete(task_ID_green);
			task_ID_green = -1;

			light_disable();

			countDown1 = red_duration;
			countDown2 = green_duration;

			if (red_input == green_input + amber_input) {
				red_duration = red_input;
				amber_duration = amber_input;
				green_duration = green_input;
			}

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

static void increase() {
	++countUp;
	if (countUp == 100) {
		countUp = 1;
	}
}

static void button_process(uint8_t *input) {
	if (button_is_pressed(BUTTON_INCREASE)) {
		SCH_add(increase, 0, 0);
	}

	if (button_is_held(BUTTON_INCREASE, 250)) {
		if (task_ID_increase == -1) {
			task_ID_increase = SCH_add(increase, 0, 500);
		}
	}

	if (button_is_released(BUTTON_INCREASE)) {
		SCH_delete(task_ID_increase);
		task_ID_increase = -1;
	}

	if (button_is_pressed(BUTTON_SET)) {
		*input = countUp;
	}
}
