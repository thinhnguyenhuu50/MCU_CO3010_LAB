#include "led7seg.h"
#include "main.h"

uint8_t led7seg_map_of_number[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};
uint8_t led7seg_index[] = {0x8, 0x4, 0x2, 0x1};
uint8_t led_7seg_index = 0;
uint8_t led_7seg[4] = {0, 0, 0, 0};

const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {1, 2, 3, 4};

void display7SEG (int num) {
	HAL_GPIO_WritePin(LED7SEG_a_GPIO_Port, LED7SEG_a_Pin, !((~led7seg_map_of_number[num]) & (1 << 0)));
	HAL_GPIO_WritePin(LED7SEG_b_GPIO_Port, LED7SEG_b_Pin, !((~led7seg_map_of_number[num]) & (1 << 1)));
	HAL_GPIO_WritePin(LED7SEG_c_GPIO_Port, LED7SEG_c_Pin, !((~led7seg_map_of_number[num]) & (1 << 2)));
	HAL_GPIO_WritePin(LED7SEG_d_GPIO_Port, LED7SEG_d_Pin, !((~led7seg_map_of_number[num]) & (1 << 3)));
	HAL_GPIO_WritePin(LED7SEG_e_GPIO_Port, LED7SEG_e_Pin, !((~led7seg_map_of_number[num]) & (1 << 4)));
	HAL_GPIO_WritePin(LED7SEG_f_GPIO_Port, LED7SEG_f_Pin, !((~led7seg_map_of_number[num]) & (1 << 5)));
	HAL_GPIO_WritePin(LED7SEG_g_GPIO_Port, LED7SEG_g_Pin, !((~led7seg_map_of_number[num]) & (1 << 6)));
}

void enable_seg(int num) {
	HAL_GPIO_WritePin(EN_3_GPIO_Port, EN_3_Pin, !((led7seg_index[num]) & (1 << 0)));
	HAL_GPIO_WritePin(EN_2_GPIO_Port, EN_2_Pin, !((led7seg_index[num]) & (1 << 1)));
	HAL_GPIO_WritePin(EN_1_GPIO_Port, EN_1_Pin, !((led7seg_index[num]) & (1 << 2)));
	HAL_GPIO_WritePin(EN_0_GPIO_Port, EN_0_Pin, !((led7seg_index[num]) & (1 << 3)));
}

void led_7seg_set_digit(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4) {
	led_7seg[0] = num1;
	led_7seg[1] = num2;
	led_7seg[2] = num3;
	led_7seg[3] = num4;
}

void led_7seg_display() {
	enable_seg(led_7seg_index);
	display7SEG(led_7seg[led_7seg_index]);
	++led_7seg_index;
	if (led_7seg_index >= 4) {
		led_7seg_index = 0;
	}
}

void update7SEG(int index){
	switch (index){
	case 0:
		//Display the first 7SEG with led_buffer[0]
		enable_seg(0);
		display7SEG(led_buffer[0]);
		break;
	case 1:
		//Display the second 7SEG with led_buffer[1]
		enable_seg(1);
		display7SEG(led_buffer[1]);
		break;
	case 2:
		//Display the third 7SEG with led_buffer[2]
		enable_seg(2);
		display7SEG(led_buffer[2]);
		break;
	case 3:
		//Display the forth 7SEG with led_buffer[3]
		enable_seg(3);
		display7SEG(led_buffer[3]);
		break;
	default:
		break;
	}
}
