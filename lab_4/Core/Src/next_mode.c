/*
 * next_mode.c
 *
 *  Created on: Nov 14, 2025
 *      Author: thinh
 */

#include "next_mode.h"
#include "global.h"
#include"button.h"

void next_mode() {
	if (button_is_pressed(BUTTON_SELLECT_MODE)) {
		switch (status) {
			case RED_AMBER:
			case RED_GREEN:
			case GREEN_RED:
			case AMBER_RED:
				light_disable();
				status = SET_RED;
				break;
			case SET_RED:
				light_disable();
				status = SET_AMBER;
				break;
			case SET_AMBER:
				light_disable();
				status = SET_GREEN;
				break;
			case SET_GREEN:
				light_disable();
				status = RED_AMBER;
				break;
			default:
				break;
		}
	}
}
