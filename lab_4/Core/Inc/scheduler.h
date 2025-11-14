/*
 * scheduler.h
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "stdint.h"

#define TICK						1 //ms
#define SCH_MAX_TASKS		40

typedef struct {
	void (*pTask) (void);
	uint32_t 			delay;
	uint32_t 			period;
	uint8_t 			runMe;

	uint32_t			taskID;
} sTasks;

void SCH_init(void);
uint8_t SCH_add(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_update(void);
void SCH_dispatch(void);
unsigned char SCH_delete(const uint8_t index);

#endif /* INC_SCHEDULER_H_ */
