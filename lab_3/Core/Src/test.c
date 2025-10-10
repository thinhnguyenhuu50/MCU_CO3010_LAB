/*
 * test.c
 *
 *  Created on: Oct 9, 2025
 *      Author: thinh
 */

#include "test.h"

static uint8_t led_blinking = 0;

void test_toggle_led_debug() {
	HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
	HAL_Delay(500);
}

void test_button() {
	if (button_is_pressed(0)) {
		HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
	}

	if (button_is_pressed(1)) {
		HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
	}

	if (button_is_pressed(2)) {
		HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
	}

	if (button_is_held(0, 250)) {
		led_blinking = 1;
	}
	else {
		led_blinking = 0;
	}

	if (timer_is_expired(1)) {
		if (led_blinking) {
			HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
		}
	}
}
