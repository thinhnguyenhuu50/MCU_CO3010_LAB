/*
 * test.c
 *
 *  Created on: Oct 9, 2025
 *      Author: thinh
 */
#include "test.h"
#include "main.h"
#include "gpio.h"
#include "software_timer.h"

void test_toggle_led_debug() {
	HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
	HAL_Delay(500);
}
