/*
 * led_matrix.c
 *
 *  Created on: Sep 7, 2025
 *      Author: Admin
 */

#include "led_matrix.h"
#include "main.h"

const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;

//uint8_t matrix_buffer[8] = {0xE7, 0xE7, 0xDB, 0xDB, 0x81, 0xBD, 0x7E, 0x7E};
uint8_t matrix_buffer[8] = {0x00,0x42,0x62,0x52,0x4a,0x46,0x42,0x00};
uint8_t row_map[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void set_row (int num) {
	HAL_GPIO_WritePin(ROW_0_GPIO_Port, ROW_0_Pin, !(row_map[num] & 1 << 0));
	HAL_GPIO_WritePin(ROW_1_GPIO_Port, ROW_1_Pin, !(row_map[num] & 1 << 1));
	HAL_GPIO_WritePin(ROW_2_GPIO_Port, ROW_2_Pin, !(row_map[num] & 1 << 2));
	HAL_GPIO_WritePin(ROW_3_GPIO_Port, ROW_3_Pin, !(row_map[num] & 1 << 3));
	HAL_GPIO_WritePin(ROW_4_GPIO_Port, ROW_4_Pin, !(row_map[num] & 1 << 4));
	HAL_GPIO_WritePin(ROW_5_GPIO_Port, ROW_5_Pin, !(row_map[num] & 1 << 5));
	HAL_GPIO_WritePin(ROW_6_GPIO_Port, ROW_6_Pin, !(row_map[num] & 1 << 6));
	HAL_GPIO_WritePin(ROW_7_GPIO_Port, ROW_7_Pin, !(row_map[num] & 1 << 7));
}

void set_col(int num) {
	HAL_GPIO_WritePin(ENM_0_GPIO_Port, ENM_0_Pin, ~(matrix_buffer[num] >> 7) & 1);
	HAL_GPIO_WritePin(ENM_1_GPIO_Port, ENM_1_Pin, ~(matrix_buffer[num] >> 6) & 1);
	HAL_GPIO_WritePin(ENM_2_GPIO_Port, ENM_2_Pin, ~(matrix_buffer[num] >> 5) & 1);
	HAL_GPIO_WritePin(ENM_3_GPIO_Port, ENM_3_Pin, ~(matrix_buffer[num] >> 4) & 1);
	HAL_GPIO_WritePin(ENM_4_GPIO_Port, ENM_4_Pin, ~(matrix_buffer[num] >> 3) & 1);
	HAL_GPIO_WritePin(ENM_5_GPIO_Port, ENM_5_Pin, ~(matrix_buffer[num] >> 2) & 1);
	HAL_GPIO_WritePin(ENM_6_GPIO_Port, ENM_6_Pin, ~(matrix_buffer[num] >> 1) & 1);
	HAL_GPIO_WritePin(ENM_7_GPIO_Port, ENM_7_Pin, ~(matrix_buffer[num] >> 0) & 1);
}

void updateLEDMatrix(int index){
	switch (index){
	case 0:
		set_row(index);
		set_col(index);
		break;
	case 1:
		set_row(index);
		set_col(index);
		break;
	case 2:
		set_row(index);
		set_col(index);
		break;
	case 3:
		set_row(index);
		set_col(index);
		break;
	case 4:
		set_row(index);
		set_col(index);
		break;
	case 5:
		set_row(index);
		set_col(index);
		break;
	case 6:
		set_row(index);
		set_col(index);
		break;
	case 7:
		set_row(index);
		set_col(index);
		break;
	default:
		set_row(index);
		set_col(index);
		break;
	}
}

void shift_left_matrix() {
	for (int i = 0; i < MAX_LED_MATRIX; ++i) {
		matrix_buffer[i] = (matrix_buffer[i] << 1) | (matrix_buffer[i] >> 7);
	}
}
