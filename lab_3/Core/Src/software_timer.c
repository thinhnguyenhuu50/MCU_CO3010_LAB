/*
 * software_timer.c
 *
 *  Created on: Sep 7, 2025
 *      Author: Admin
 */

#include "software_timer.h"
#include "tim.h"

#define NUMBER_OF_TIMERS 10

uint8_t timer_flag = 0;

struct TimerStruct
{
	uint8_t counter;
	uint8_t flag;
	uint8_t origin;
};

static struct TimerStruct timer[NUMBER_OF_TIMERS];

/* Function */
void timer_init() {
	HAL_TIM_Base_Start_IT(&htim2);
}

void timer_set(int index, int duration){
	timer[index].origin = duration / TIMER_PERIOD;
	timer[index].counter = duration / TIMER_PERIOD;
	timer[index].flag = 0;
}

void timer_run(){
	for (int i = 0; i < NUMBER_OF_TIMERS; ++i) {
		if (timer[i].counter > 0) {
			--timer[i].counter;
			if (timer[i].counter <= 0) {
				timer[i].flag = 1;
			}
		}
	}
}

int timer_is_expired(int index) {
	if (timer[index].flag) {
		timer[index].counter = timer[index].origin;
		timer[index].flag = 0;
		return 1;
	}
	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		timer_flag = 1;
	}
}
