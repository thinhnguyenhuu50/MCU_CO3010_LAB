/*
 * button.h
 *
 *  Created on: Oct 5, 2023
 *      Author: KAI
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NORMAL_STATE SET
#define PRESSED_STATE RESET

extern int button1_flag;
int isButton1Pressed();
void getKeyInput();

//------------------

uint8_t button_is_pressed();
uint8_t button_is_held(int duration);
void button_scan();

#endif /* INC_BUTTON_H_ */

