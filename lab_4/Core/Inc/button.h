#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

void button_scan();
uint8_t button_is_pressed(uint8_t index);
uint8_t button_is_held(uint8_t index, int duration);
uint8_t button_is_released(uint8_t index);

#endif /* INC_BUTTON_H_ */

