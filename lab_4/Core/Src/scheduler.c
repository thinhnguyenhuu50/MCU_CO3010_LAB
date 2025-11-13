/*
 * scheduler.c
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#include "scheduler.h"
#include "tim.h"
sTasks SCH_tasks_G[SCH_MAX_TASKS];

void SCH_init(void) {
	HAL_TIM_Base_Start_IT(&htim2);
}

unsigned char SCH_add(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD) {
	unsigned char Index = 0;

	while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
	{
		Index++;
	}

	if (Index == SCH_MAX_TASKS)
	{
		//        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}

	SCH_tasks_G[Index].pTask  = pFunction;
	SCH_tasks_G[Index].delay  = DELAY/TICK;
	SCH_tasks_G[Index].period = PERIOD/TICK;
	SCH_tasks_G[Index].runMe  = 0;

	return Index;
}

void SCH_update(void)
{
    unsigned char Index;

    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].pTask) {
            if (SCH_tasks_G[Index].delay == 0) {
                SCH_tasks_G[Index].runMe += 1;

                if (SCH_tasks_G[Index].period) {
                    SCH_tasks_G[Index].delay = SCH_tasks_G[Index].period;
                }
            }
            else {
                SCH_tasks_G[Index].delay -= 1;
            }
        }
    }
}


void SCH_dispatch(void)
{
    unsigned char Index;

    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if (SCH_tasks_G[Index].runMe > 0)
        {
            (*SCH_tasks_G[Index].pTask)();
            SCH_tasks_G[Index].runMe -= 1;

            if (SCH_tasks_G[Index].period == 0)
            {
                SCH_delete(Index);
            }
        }
    }

//    SCH_Report_Status();
//    SCH_Go_To_Sleep();
}

unsigned char SCH_delete(const uint8_t TASK_INDEX)
{
    unsigned char Return_code = 0;

    if (SCH_tasks_G[TASK_INDEX].pTask == 0)
    {
//        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
//        Return_code = RETURN_ERROR;
    }
    else
    {
//        Return_code = RETURN_NORMAL;
    }

    SCH_tasks_G[TASK_INDEX].pTask  = 0x0000;
    SCH_tasks_G[TASK_INDEX].delay  = 0;
    SCH_tasks_G[TASK_INDEX].period = 0;
    SCH_tasks_G[TASK_INDEX].runMe  = 0;

    return Return_code;
}


void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim ) {
	SCH_update();
}
