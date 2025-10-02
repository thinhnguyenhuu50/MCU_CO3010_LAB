/*
 * led7seg.h
 *
 *  Created on: Sep 4, 2025
 *      Author: Admin
 */

#ifndef INC_LED7SEG_H_
#define INC_LED7SEG_H_

#include "main.h"

void display7SEG (int num);
void enable_seg(int num);

void led_7seg_set_digit(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4);
void led_7seg_display();

void led_7seg_toggle_dot();
#endif /* INC_LED7SEG_H_ */
