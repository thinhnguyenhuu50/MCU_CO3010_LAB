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
#include "software_timer.h"

#define DEFAULT_RED_TIME  	5
#define DEFAULT_AMBER_TIME  2
#define DEFAULT_GREEN_TIME  3

// Timer indexes
#define TIMER_ONE_SECOND		0
#define TIMER_LED_BLINKING		1
#define TIMER_INCREASE_VALUE 	2

typedef enum {INIT, NORMAL, SET_RED, SET_AMBER, SET_GREEN} status_Mode_t;
/**
 * The light states of each phase are applied for the both roads as follow:
 * P0: R1=RED, R2=GREEN     | P1: R1=RED, R2=AMBER
 * P2: R1=GREEN, R2=RED     | P3: R1=AMBER, R2=RED
 */
typedef enum {RED_GREEN, RED_AMBER, GREEN_RED, AMBER_RED} status_Traffic_light_t;
enum {BUTTON_SELLECT_MODE, BUTTON_INCREASE, BUTTON_SET};
typedef enum {GREEN, AMBER, RED} color_t;

static status_Mode_t status_Mode = INIT;
static status_Traffic_light_t status_Traffic_light = RED_GREEN;

static uint8_t map_of_led_state[3] = {0x01, 0x02, 0x04};

static uint8_t green_duration 	= DEFAULT_GREEN_TIME;
static uint8_t amber_duration 	= DEFAULT_AMBER_TIME;
static uint8_t red_duration 	= DEFAULT_RED_TIME;

// Read from user via buttons
static uint8_t countUp = 1;
static uint8_t green_input = 1;
static uint8_t amber_input = 1;
static uint8_t red_input = 1;

static uint8_t countdown1 = 0;
static uint8_t countdown2 = 0;
// Forward declaration
static void set_led7seg_Road1(uint8_t number);
static void set_led7seg_Road2(uint8_t number);

static void light_set1(color_t color);
static void light_set2(color_t color);
static void light_disable();

static void traffic_light_run();

static void button_process(uint8_t *input);

// APIs
void app_Set_timer() {
	timer_set(TIMER_ONE_SECOND, 1000);
	timer_set(TIMER_LED_BLINKING, 500);
	timer_set(TIMER_INCREASE_VALUE, 100);
}

void app_Select_mode() {
	switch (status_Mode) {
	case INIT:
		light_disable();
		countdown1 = red_duration;
		countdown2 = green_duration;
		status_Mode = NORMAL;
		break;
	case NORMAL:
		traffic_light_run();

		// Condition to move to the next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			light_disable();
			red_input = red_duration;
			countUp = 1;
			status_Mode = SET_RED;
		}
		break;
	case SET_RED:
		if (timer_is_expired(TIMER_LED_BLINKING)) {
			HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
			HAL_GPIO_TogglePin(RED2_GPIO_Port, RED2_Pin);
		}

		// Button process
		button_process(&red_input);

		// Display value and mode number
		set_led7seg_Road1(countUp);
		set_led7seg_Road2(2);

		// Condition to move to the next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status_Mode = SET_AMBER;
		}
		break;
	case SET_AMBER:
		// Debug
		light_set1(AMBER);

		set_led7seg_Road1(13);
		set_led7seg_Road2(13);

		// Condition to move to next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status_Mode = SET_GREEN;
		}
		break;
	case SET_GREEN:
		// Debug
		light_set1(GREEN);

		set_led7seg_Road1(14);
		set_led7seg_Road2(14);

		// Condition to move to the next state
		if (button_is_pressed(BUTTON_SELLECT_MODE)) {
			status_Mode = NORMAL;
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

static void light_disable() {
	HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, 1);
	HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, 1);
	HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, 1);
	HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, 1);
	HAL_GPIO_WritePin(AMBER2_GPIO_Port, AMBER2_Pin, 1);
	HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, 1);
}

static void traffic_light_run() {
	switch (status_Traffic_light) {
	case RED_GREEN:
		light_set1(RED);
		light_set2(GREEN);

		if (countdown2 == 0) {
			status_Traffic_light = RED_AMBER;

			countdown2 = amber_duration;
		}
		break;
	case RED_AMBER:
		light_set1(RED);
		light_set2(AMBER);

		if (countdown2 == 0) {
			status_Traffic_light = GREEN_RED;

			countdown1 = green_duration;
			countdown2 = red_duration;
		}
		break;
	case GREEN_RED:
		light_set1(GREEN);
		light_set2(RED);

		if (countdown1 == 0) {
			status_Traffic_light = AMBER_RED;

			countdown1 = amber_duration;
		}
		break;
	case AMBER_RED:
		light_set1(AMBER);
		light_set2(RED);

		if (countdown1 == 0) {
			status_Traffic_light = RED_GREEN;
			countdown1 = red_duration;
			countdown2 = green_duration;
		}
		break;
	default:
		break;
	}

	if (timer_is_expired(TIMER_ONE_SECOND)) {
		--countdown1;
		--countdown2;
	}

	set_led7seg_Road1(countdown1);
	set_led7seg_Road2(countdown2);
}

static void button_process(uint8_t *input) {
	if (button_is_pressed(BUTTON_INCREASE)) {
		++countUp;
		if (countUp == 100) {
			countUp = 1;
		}
	}

	if (button_is_held(BUTTON_INCREASE, 250) && timer_is_expired(TIMER_INCREASE_VALUE)) {
		++countUp;
		if (countUp == 100) {
			countUp = 1;
		}
	}

	if (button_is_pressed(BUTTON_SET)) {
		red_input = countUp;
	}
}
