/*
 * scheduler.c
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#include "scheduler.h"
#include "tim.h"
#include "stdint.h"

typedef struct {
	void 					(*pTask) (void);
	uint32_t 			delay;
	uint32_t 			period;
	uint8_t 			runMe;
} sTasks;
static sTasks SCH_tasks_G[SCH_MAX_TASKS];
static uint8_t SCH_size = 0;

// Forward declaration
static void SCH_delete_head(void);

void SCH_init(void) {
	HAL_TIM_Base_Start_IT(&htim2);

	SCH_size = 0;
	for (int i = 0; i < SCH_MAX_TASKS; ++i) {
		SCH_tasks_G[i].pTask  = 0;
		SCH_tasks_G[i].delay  = 0;
		SCH_tasks_G[i].period = 0;
		SCH_tasks_G[i].runMe  = 0;
	}
}

void SCH_add(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
	if (SCH_size >= SCH_MAX_TASKS) {
		return;  // queue full
	}

	uint32_t remaining = DELAY/TICK;   // remaining delay for new task
	uint32_t p = PERIOD/TICK;
	uint8_t  idx = 0;

	for (int i = 0; i < SCH_size; ++i) {
		if (SCH_tasks_G[i].pTask == pFunction) {
			return;
		}
	}

	while (idx < SCH_size && SCH_tasks_G[idx].pTask != 0 && remaining > SCH_tasks_G[idx].delay) {
		remaining -= SCH_tasks_G[idx].delay;
		++idx;
	}

	for (int j = SCH_size; j > idx; --j) {
		SCH_tasks_G[j] = SCH_tasks_G[j - 1];
	}

	SCH_tasks_G[idx].pTask  = pFunction;
	SCH_tasks_G[idx].delay  = remaining;
	SCH_tasks_G[idx].period = p;
	SCH_tasks_G[idx].runMe  = 0;

	if (idx + 1 < SCH_size + 1) {
		SCH_tasks_G[idx + 1].delay -= remaining;
	}

	++SCH_size;
}

void SCH_update(void) {
	if (SCH_size == 0) {
		return; // no task
	}

	if (SCH_tasks_G[0].delay > 0) {
		--SCH_tasks_G[0].delay;
	}

	if (SCH_tasks_G[0].delay == 0) {
		++SCH_tasks_G[0].runMe;
	}
}

void SCH_dispatch(void) {
	// Run all tasks whose delay has reached 0
	while (SCH_size > 0 && SCH_tasks_G[0].runMe > 0 && SCH_tasks_G[0].pTask != 0) {
		(*SCH_tasks_G[0].pTask)();
		--SCH_tasks_G[0].runMe;

		if (SCH_tasks_G[0].period > 0) {
			void (*f)(void) = SCH_tasks_G[0].pTask;
			uint32_t p = SCH_tasks_G[0].period;

			SCH_delete_head();                // remove old instance
			SCH_add(f, p*TICK, p*TICK);        // reinsert with its period as new delay
		} else {
			SCH_delete_head();
		}
	}
	__WFI();
	//    SCH_Report_Status();
	//    SCH_Go_To_Sleep();
}

static void SCH_delete_head(void) {
	if (SCH_size == 0) return;

	if (SCH_size == 1) {
		SCH_tasks_G[0].pTask = 0;
		SCH_tasks_G[0].delay = 0;
		SCH_tasks_G[0].period = 0;

		--SCH_size;

		return;
	}

	for (uint8_t i = 0; i < SCH_size - 1; ++i) {
		SCH_tasks_G[i] = SCH_tasks_G[i + 1];
	}

	--SCH_size;
}

void SCH_delete(void (*pFunction) (void)) {
	if (SCH_size == 0) return;

	uint8_t idx = 0;

	while (idx < SCH_size && SCH_tasks_G[idx].pTask != pFunction) {
		++idx;
	}

	if (idx == SCH_size) {
		return;
	}

	if (idx + 1 < SCH_size) {
		SCH_tasks_G[idx + 1].delay += SCH_tasks_G[idx].delay;
	}

	for (uint8_t i = idx; i < SCH_size - 1; ++i) {
		SCH_tasks_G[i] = SCH_tasks_G[i + 1];
	}

	--SCH_size;
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim ) {
	SCH_update();
}
