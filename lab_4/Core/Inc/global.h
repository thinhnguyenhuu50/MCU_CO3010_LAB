/*
 * global.h
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_
#include "stdint.h"

typedef enum {INIT, COUNT_DOWN, RED_GREEN, RED_AMBER, GREEN_RED, AMBER_RED,
							SET_RED, SET_AMBER, SET_GREEN} fsm_status_t;
typedef enum {GREEN, AMBER, RED} color_t;
enum {BUTTON_SELLECT_MODE, BUTTON_INCREASE, BUTTON_SET};

extern fsm_status_t status;

extern uint8_t green_duration;
extern uint8_t amber_duration;
extern uint8_t red_duration;

extern uint8_t countUp;
extern uint8_t green_input;
extern uint8_t amber_input;
extern uint8_t red_input;

void light_disable();

void set_led7seg_Road1(uint8_t number);
void set_led7seg_Road2(uint8_t number);

#endif /* INC_GLOBAL_H_ */
