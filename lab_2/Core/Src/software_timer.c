/*
 * software_timer.c
 *
 *  Created on: Sep 7, 2025
 *      Author: Admin
 */

#include "software_timer.h"
#include "tim.h"

#define NUMBER_OF_TIMERS 10
#define TIMER_CYCLE 10; // 10 ms period

struct TimerStruct
{
	int counter;
	int flag;
};

static struct TimerStruct timer[NUMBER_OF_TIMERS];

/* Function */
void timer_set(int index, int duration){
	timer[index].counter = duration / TIMER_CYCLE;
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
		return 1;
	}
	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	timer_run();
}
