/*
 * global.c
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#include "global.h"

#define DEFAULT_RED_TIME  	5
#define DEFAULT_AMBER_TIME  2
#define DEFAULT_GREEN_TIME  3

fsm_status_t status = INIT;

uint8_t green_duration 	= DEFAULT_GREEN_TIME;
uint8_t amber_duration 	= DEFAULT_AMBER_TIME;
uint8_t red_duration 	= DEFAULT_RED_TIME;
