/*
 * global.h
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_
#include "stdint.h"

typedef enum {INIT, COUNT_DOWN, RED_GREEN, RED_AMBER, GREEN_RED, AMBER_RED} fsm_status_t;
typedef enum {GREEN, AMBER, RED} color_t;

extern fsm_status_t status;

extern uint8_t green_duration;
extern uint8_t amber_duration;
extern uint8_t red_duration;

#endif /* INC_GLOBAL_H_ */
