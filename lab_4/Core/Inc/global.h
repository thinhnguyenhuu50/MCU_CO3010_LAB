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

extern int8_t task_ID_countdown;

void light_disable();

#endif /* INC_GLOBAL_H_ */
