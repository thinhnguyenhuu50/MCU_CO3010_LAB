/*
 * led7seg.c
 *
 *  Created on: Sep 4, 2025
 *      Author: Admin
 */
#include <led7seg.h>
#include "main.h"

uint8_t led7seg_map_of_number[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};
uint8_t led7seg_index[] = {0x2, 0x1};
uint8_t led_7seg_index = 0;
uint8_t led_7seg[2] = {0, 0};

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
	HAL_GPIO_WritePin(EN_1_GPIO_Port, EN_1_Pin, !((led7seg_index[num]) & (1 << 0)));
	HAL_GPIO_WritePin(EN_0_GPIO_Port, EN_0_Pin, !((led7seg_index[num]) & (1 << 1)));
}

void led_7seg_set_digit(uint8_t num1, uint8_t num2) {
	led_7seg[0] = num1;
	led_7seg[1] = num2;
}

void led_7seg_display() {
	enable_seg(led_7seg_index);
	display7SEG(led_7seg[led_7seg_index]);
	++led_7seg_index;
	if (led_7seg_index >= 2) {
		led_7seg_index = 0;
	}
}


