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

typedef enum {INIT, NORMAL, SET_RED, SET_AMBER, SET_GREEN} Status;
enum {BUTTON_SELLECT_MODE, BUTTON_MODIFY, BUTTON_SET};

static Status status = INIT;

// Forward declaration
static void set_led7seg_Road1(uint8_t number);
static void set_led7seg_Road2(uint8_t number);
static void set_led7seg_CurrentMode(uint8_t number);

// APIs
void select_mode() {
	switch (status) {
	case INIT:

		status = NORMAL;
		break;
	case NORMAL:
		// debug
		set_led7seg_CurrentMode(1);

		// Condition to move to next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status = SET_RED;
		}
		break;
	case SET_RED:
		// Debug
		set_led7seg_CurrentMode(2);

		// Condition to move to next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status = SET_AMBER;
		}
		break;
	case SET_AMBER:
		// Debug
		set_led7seg_CurrentMode(3);

		// Condition to move to next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status = SET_GREEN;
		}
		break;
	case SET_GREEN:
		// Debug
		set_led7seg_CurrentMode(4);

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

static void set_led7seg_CurrentMode(uint8_t number){
	led7seg_set(4, number/10);
	led7seg_set(5, number%10);
}
