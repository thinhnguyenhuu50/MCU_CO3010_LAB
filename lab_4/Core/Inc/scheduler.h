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

void SCH_init(void);
void SCH_add(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_update(void);
void SCH_dispatch(void);
void SCH_delete(void (*pFunction) (void));

#endif /* INC_SCHEDULER_H_ */
