/*
 * fsm_auto.c
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#include "fsm_auto.h"
#include "global.h"
#include "stdint.h"
#include "gpio.h"
#include "led7seg.h"
#include "scheduler.h"
#include "button.h"

static uint8_t map_of_led_state[3] = {0x01, 0x02, 0x04};

// Forward declaration
static void countdown();

static void light_set1(color_t color);
static void light_set2(color_t color);

static void next_mode();

void fsm_auto() {
	switch (status) {
	case INIT:
		light_disable();

		countDown1 = red_duration;
		countDown2 = green_duration;

		set_led7seg_Road1(countDown1);
		set_led7seg_Road2(countDown2);

		SCH_add(countdown, 1000, 1000);

		status = RED_GREEN;
		break;

	case RED_GREEN:
		light_set1(RED);
		light_set2(GREEN);

		if (countDown2 <= 0) {
			countDown2 = amber_duration;
			set_led7seg_Road2(countDown2);

			status = RED_AMBER;
		}

		next_mode();
		break;

	case RED_AMBER:
		light_set1(RED);
		light_set2(AMBER);

		if (countDown2 <= 0) {
			countDown1 = green_duration;
			countDown2 = red_duration;

			set_led7seg_Road1(countDown1);
			set_led7seg_Road2(countDown2);

			status = GREEN_RED;
		}

		next_mode();
		break;

	case GREEN_RED:
		light_set1(GREEN);
		light_set2(RED);

		if (countDown1 <= 0) {
			countDown1 = amber_duration;

			set_led7seg_Road1(countDown1);

			status = AMBER_RED;
		}

		next_mode();
		break;

	case AMBER_RED:
		light_set1(AMBER);
		light_set2(RED);

		if (countDown1 <= 0) {
			countDown1 = red_duration;
			countDown2 = green_duration;

			set_led7seg_Road1(countDown1);
			set_led7seg_Road2(countDown2);

			status = RED_GREEN;
		}

		next_mode();
		break;

	default:
		break;
	}
}

// Supported functions
static void light_set1(color_t color) {
	HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, !(map_of_led_state[color] & (1<<0)));
	HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, !(map_of_led_state[color] & (1<<1)));
	HAL_GPIO_WritePin(RED1_GPIO_Port, 	RED1_Pin, 	!(map_of_led_state[color] & (1<<2)));
}

static void light_set2(color_t color) {
	HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, !(map_of_led_state[color] & (1<<0)));
	HAL_GPIO_WritePin(AMBER2_GPIO_Port, AMBER2_Pin, !(map_of_led_state[color] & (1<<1)));
	HAL_GPIO_WritePin(RED2_GPIO_Port,   RED2_Pin,   !(map_of_led_state[color] & (1<<2)));
}

static void countdown() {
	--countDown1;
	--countDown2;
	if (countDown1 != 0) {
		set_led7seg_Road1(countDown1);
	}

	if (countDown2 != 0) {
		set_led7seg_Road2(countDown2);
	}
}

static void next_mode() {
	if (button_is_pressed(BUTTON_SELLECT_MODE)) {
		SCH_delete(countdown);

		light_disable();

		red_input = red_duration;
		countUp = 1;

		status = SET_RED;
	}
}
