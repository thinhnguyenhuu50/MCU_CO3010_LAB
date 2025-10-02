/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SPEED_FOR_SECOND	1000 // Millisecond unit
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int led_state = 0x0;
int hour = 0; // 0-11
int minute = 0; // 0-59
int second = 0; // 0-59

int old_state[3] = {0, 0, 0}; // second, minute, hour
int new_state[3] = {0, 0, 0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void clearAllClock();
void setNumberOnClock(int num);
void clearNumberOnClock(int num);
void update_state();

void test_basic_function();
void clock_display();
void update_clock();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, 1);
	clearAllClock();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
		clock_display();
		HAL_Delay(SPEED_FOR_SECOND);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_DEBUG_Pin|LED_0_Pin|LED_1_Pin|LED_2_Pin
                          |LED_3_Pin|LED_4_Pin|LED_5_Pin|LED_6_Pin
                          |LED_7_Pin|LED_8_Pin|LED_9_Pin|LED_10_Pin
                          |LED_11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_DEBUG_Pin LED_0_Pin LED_1_Pin LED_2_Pin
                           LED_3_Pin LED_4_Pin LED_5_Pin LED_6_Pin
                           LED_7_Pin LED_8_Pin LED_9_Pin LED_10_Pin
                           LED_11_Pin */
  GPIO_InitStruct.Pin = LED_DEBUG_Pin|LED_0_Pin|LED_1_Pin|LED_2_Pin
                          |LED_3_Pin|LED_4_Pin|LED_5_Pin|LED_6_Pin
                          |LED_7_Pin|LED_8_Pin|LED_9_Pin|LED_10_Pin
                          |LED_11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void clearAllClock() {
	led_state = 0xFFF;
	update_state();
}

void setNumberOnClock(int num) {
	led_state = led_state & ~(1 << num);
	update_state();
}

void clearNumberOnClock(int num) {
	led_state = led_state | (1 << num);
	update_state();
}

void update_state() {
	HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, !(~led_state & (1 << 0)));
	HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, !(~led_state & (1 << 1)));
	HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, !(~led_state & (1 << 2)));
	HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, !(~led_state & (1 << 3)));
	HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, !(~led_state & (1 << 4)));
	HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, !(~led_state & (1 << 5)));
	HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, !(~led_state & (1 << 6)));
	HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, !(~led_state & (1 << 7)));
	HAL_GPIO_WritePin(LED_8_GPIO_Port, LED_8_Pin, !(~led_state & (1 << 8)));
	HAL_GPIO_WritePin(LED_9_GPIO_Port, LED_9_Pin, !(~led_state & (1 << 9)));
	HAL_GPIO_WritePin(LED_10_GPIO_Port, LED_10_Pin, !(~led_state & (1 << 10)));
	HAL_GPIO_WritePin(LED_11_GPIO_Port, LED_11_Pin, !(~led_state & (1 << 11)));
}

void test_basic_function() {
	clearAllClock();
	for (int i = 0; i < 12; ++i) {
		setNumberOnClock(i);
		HAL_Delay(250);
	}

	for (int i = 0; i < 12; ++i) {
		clearNumberOnClock(i);
		HAL_Delay(250);
	}
}

void clock_display() { // Note: Proteus messes with modulo operator (%)
	update_clock();
	++second;
	if (second >= 60) {
		second = 0;
	}

	if (second == 0) {
		++minute;
		if (minute >= 60) {
			minute = 0;
		}
		if (minute == 0) {
			++hour;
			if (hour >= 12) {
				hour = 0;
			}
		}
	}
}

void update_clock() {
	new_state[0] = second/5;
	new_state[1] = minute/5;
	new_state[2] = hour;

	for (int i = 0; i < 3; ++i) {
		setNumberOnClock(new_state[i]);
	}

	for (int i = 0; i < 3; ++i) {
		int remove_flag = 1;
		for (int j = 0; j < 3; ++j) {
			if (old_state[i] == new_state[j]) {
				remove_flag = 0;
				break;
			}
		}
		if (remove_flag == 1) {
			clearNumberOnClock(old_state[i]);
		}
		old_state[i] = new_state[i];
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
