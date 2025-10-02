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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led7seg.h"
#include "software_timer.h"
#include "led_matrix.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIMER_LED_DEBUG 		0
#define TIMER_7SEG_DOT 			1
#define TIMER_7SEG_LED 			2
#define TIMER_LED_MATRIX		3
#define TIMER_LED_MATRIX_SHIFT 	4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int hour = 15, minute = 8, second = 50;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void updateClockBuffer();
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
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	updateClockBuffer();
	update7SEG(index_led++);
	HAL_GPIO_WritePin(DOT_GPIO_Port, DOT_Pin, 0);
	timer_set(TIMER_LED_DEBUG, 500);
	timer_set(TIMER_7SEG_DOT, 1000);
	timer_set(TIMER_7SEG_LED, 250);
	timer_set(TIMER_LED_MATRIX, 250);
	timer_set(TIMER_LED_MATRIX_SHIFT, 2000);
	/**
	 * Report 1: If this line is missed, the software timer will not work! :O
	 */
	/**
	 * Report 2: 1 is smaller than 10, which is the current interrupt period; thus, the counter still ends up as 0
	 * after the division. Bangggg!
	 */
	/**
	 * Report 3: The counter can be set as 1 which means that the flag has a chance to be raised, resulting in a proper
	 * timer interrupt. BUuumm!
	 */
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		if (timer_is_expired(TIMER_7SEG_DOT)) {
			led_7seg_toggle_dot();

			second++;
			if (second >= 60){
				second = 0;
				minute++;
			}
			if (minute >= 60){
				minute = 0;
				hour++;
			}
			if (hour >= 24){
				hour = 0;
			}

			timer_set(TIMER_7SEG_DOT, 1000);
		}

		if (timer_is_expired(TIMER_7SEG_LED)) {
			updateClockBuffer();
			update7SEG(index_led++);
			if (index_led >= 4) {
				index_led = 0;
			}

			timer_set(TIMER_7SEG_LED, 250);
		}

		if(timer_is_expired(TIMER_LED_DEBUG)){
			HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
			timer_set(TIMER_LED_DEBUG, 500);
		}

		if (timer_is_expired(TIMER_LED_MATRIX)) {
			updateLEDMatrix(index_led_matrix++);
			if (index_led_matrix == 8) {
				index_led_matrix = 0;
			}
			timer_set(TIMER_LED_MATRIX, 250);
		}

		if (timer_is_expired(TIMER_LED_MATRIX_SHIFT)) {
			shift_left_matrix();
			timer_set(TIMER_LED_MATRIX_SHIFT, 2000);
		}
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

/* USER CODE BEGIN 4 */
void updateClockBuffer()
{
	led_buffer[0] = hour / 10;
	led_buffer[1] = hour % 10;
	led_buffer[2] = minute / 10;
	led_buffer[3] = minute % 10;
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
