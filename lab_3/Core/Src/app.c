/*
 * app.c
 *
 *  Created on: Oct 23, 2025
 *      Author: thinh
 */
#include "app.h"
#include "main.h"
#include "gpio.h"
#include "button.h"
#include "led7seg.h"

typedef enum {INIT, NORMAL, SET_RED, SET_AMBER, SET_GREEN} Status_t;
enum {BUTTON_SELLECT_MODE, BUTTON_MODIFY, BUTTON_SET};
typedef enum {GREEN, AMBER, RED} Color_t;

static Status_t status = INIT;

uint8_t map_of_led_state[3] = {0x01, 0x02, 0x04};

// Forward declaration
static void set_led7seg_Road1(uint8_t number);
static void set_led7seg_Road2(uint8_t number);

static void light_set1(Color_t color);
static void light_disable();

// APIs
void select_mode() {
	switch (status) {
	case INIT:
		light_disable();
		status = NORMAL;
		break;
	case NORMAL:
		// debug
		set_led7seg_Road1(11);
		set_led7seg_Road2(11);

		// Condition to move to next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status = SET_RED;
		}
		break;
	case SET_RED:
		// Debug
		light_set1(RED);

		set_led7seg_Road1(12);
		set_led7seg_Road2(12);

		// Condition to move to next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status = SET_AMBER;
		}
		break;
	case SET_AMBER:
		// Debug
		light_set1(AMBER);

		set_led7seg_Road1(13);
		set_led7seg_Road2(13);

		// Condition to move to next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status = SET_GREEN;
		}
		break;
	case SET_GREEN:
		// Debug
		light_set1(GREEN);

		set_led7seg_Road1(14);
		set_led7seg_Road2(14);

		// Condition to move to next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status = NORMAL;
		}
		break;
	default:
		break;
	}
}

// Supported functions
static void set_led7seg_Road1(uint8_t number){
	led7seg_set(0, number/10);
	led7seg_set(1, number%10);
}

static void set_led7seg_Road2(uint8_t number){
	led7seg_set(2, number/10);
	led7seg_set(3, number%10);
}

static void light_set1(Color_t color) {
	HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, !(map_of_led_state[color] & (1<<0)));
	HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, !(map_of_led_state[color] & (1<<1)));
	HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, !(map_of_led_state[color] & (1<<2)));
}

static void light_disable() {
	HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, 1);
	HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, 1);
	HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, 1);
	HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, 1);
	HAL_GPIO_WritePin(AMBER2_GPIO_Port, AMBER2_Pin, 1);
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, 1);
}
