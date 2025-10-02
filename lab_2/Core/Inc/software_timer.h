/*
 * software_timer.h
 *
 *  Created on: Sep 7, 2025
 *      Author: Admin
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

void timer_set(int idx, int counter);
void timer_run();
int timer_is_expired(int idx);

#endif /* INC_SOFTWARE_TIMER_H_ */
