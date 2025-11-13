/*
 * global.h
 *
 *  Created on: Nov 13, 2025
 *      Author: thinh
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

typedef enum {INIT, COUNT_DOWN, RED_GREEN, RED_AMBER, GREEN_RED, AMBER_RED} fsm_status_t;
extern fsm_status_t status;

#endif /* INC_GLOBAL_H_ */
