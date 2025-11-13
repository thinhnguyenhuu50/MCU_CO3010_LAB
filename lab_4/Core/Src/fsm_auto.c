/*
 * fsm_auto.c
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#include "fsm_auto.h"
#include "global.h"
#include "stdint.h"

static uint8_t countDown1 = 0;
static uint8_t countDown2 = 0;

void fsm_auto() {
	switch (status) {
		case COUNT_DOWN:

			break;

		case RED_GREEN:
			break;
		case RED_AMBER:
			break;
		case GREEN_RED:
			break;
		case AMBER_RED:
			break;
		default:
			break;
	}
}

