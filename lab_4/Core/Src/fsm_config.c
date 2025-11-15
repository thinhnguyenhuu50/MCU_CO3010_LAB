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

//static int8_t task_ID_red = -1, task_ID_amber = -1, task_ID_green = -1, task_ID_increase = -1;

// Forward declaration
static void button_process(uint8_t *input);

void fsm_config() {
	switch (status) {
	case SET_RED:
		button_process(&red_input);

		// Display value and mode number
		set_led7seg_Road1(countUp);
		set_led7seg_Road2(2);

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			SCH_delete(blink_red);
			SCH_add(blink_amber, 0, 500);

			light_disable();

			amber_input = amber_duration;
			countUp = 1;

			status = SET_AMBER;
		}
		break;
	case SET_AMBER:
		button_process(&amber_input);

		// Display value and mode number
		set_led7seg_Road1(countUp);
		set_led7seg_Road2(3);

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			SCH_delete(blink_amber);
			SCH_add(blink_green, 0, 500);

			light_disable();

			green_input = green_duration;
			countUp = 1;

			status = SET_GREEN;
		}

		break;
	case SET_GREEN:
		button_process(&green_input);

		// Display value and mode number
		set_led7seg_Road1(countUp);
		set_led7seg_Road2(4);

		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			SCH_delete(blink_green);

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

static void increase() {
	++countUp;
	if (countUp == 100) {
		countUp = 1;
	}
}

static void button_process(uint8_t *input) {
	if (button_is_pressed(BUTTON_INCREASE)) {
		increase();
	}

	if (button_is_held(BUTTON_INCREASE, 250)) {
		SCH_add(increase, 0, 500);
	}

	if (button_is_released(BUTTON_INCREASE)) {
		SCH_delete(increase);
	}

	if (button_is_pressed(BUTTON_SET)) {
		*input = countUp;
	}
}
