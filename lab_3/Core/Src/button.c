#include "button.h"

#define NUMBER_OF_BUTTONS 3

// Private
static struct {
	uint8_t first_state; // active low
	uint8_t second_state;
	uint8_t third_state;

	GPIO_TypeDef *port;
	uint16_t pin;
} debounce[NUMBER_OF_BUTTONS] = {
		{1, 1, 1, BUTTON_0_GPIO_Port, BUTTON_0_Pin},
		{1, 1, 1, BUTTON_1_GPIO_Port, BUTTON_1_Pin},
		{1, 1, 1, BUTTON_2_GPIO_Port, BUTTON_2_Pin}
};

static int button_count[NUMBER_OF_BUTTONS];

static uint8_t button_flag = 0;

// Function
uint8_t button_is_pressed(uint8_t index) {
	if (button_count[index] == 1 && button_flag) {
		button_flag = 0;
		return 1;
	}
	return 0;
}

uint8_t button_is_held(uint8_t index, int duration) {
	if (button_count[index] >= duration / TIMER_PERIOD && button_flag) {
		button_flag = 0;
		return 1;
	}
	return 0;
}

void button_scan() {
	for (int i = 0; i < NUMBER_OF_BUTTONS; ++i) {
		debounce[i].first_state = debounce[i].second_state;
		debounce[i].second_state = debounce[i].third_state;

		debounce[i].third_state = HAL_GPIO_ReadPin(debounce[i].port, debounce[i].pin);
		if ((debounce[i].first_state == debounce[i].second_state) && (debounce[i].second_state == debounce[i].third_state)){
			if (debounce[i].third_state == 0) {
				button_count[i] += 1;
				button_flag = 1;
				return;
			}
		}
		button_count[i] = 0;
	}
}

