#ifndef INC_LED7SEG_H_
#define INC_LED7SEG_H_

#include "main.h"

extern int led_buffer[4];
extern int index_led;

void display7SEG (int num);
void enable_seg(int num);

void led_7seg_set_digit(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4);
void led_7seg_display();

void update7SEG(int index);
#endif /* INC_LED7SEG_H_ */
