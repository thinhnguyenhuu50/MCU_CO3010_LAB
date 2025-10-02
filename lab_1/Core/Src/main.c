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
#define RED_YELLOW	2
#define GREEN_RED	4
#define YELLOW_RED	7
#define RED_GREEN	9

#define RED			4
#define YELLOW		2
#define GREEN		1

#define ON 		0
#define OFF 	1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t led7seg_map_of_number[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void light_set1 (uint8_t n);
void light_set2 (uint8_t n);
void display7SEG (int num);
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
	uint8_t counter = 9; // To begin with red light
	HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, 1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
		switch (counter) {
		case RED_GREEN:
			light_set1(RED);
			light_set2(GREEN);
			break;
		case RED_YELLOW:
			light_set1(RED);
			light_set2(YELLOW);
			break;
		case GREEN_RED:
			light_set1(GREEN);
			light_set2(RED);
			break;
		case YELLOW_RED:
			light_set1(YELLOW);
			light_set2(RED);
			break;
		default:
			break;
		}
		counter = (counter + 1) % 10;
		display7SEG(counter);
		HAL_Delay(1000);
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
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LED_DEBUG_Pin|LED_YELLOW1_Pin|LED_GREEN1_Pin|LED_RED2_Pin
			|LED_YELLOW2_Pin|LED_GREEN2_Pin|LED_RED1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LED7SEG_a_Pin|LED7SEG_b_Pin|LED7SEG_c_Pin|LED7SEG_d_Pin
			|LED7SEG_e_Pin|LED7SEG_f_Pin|LED7SEG_g_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : LED_DEBUG_Pin LED_YELLOW1_Pin LED_GREEN1_Pin LED_RED2_Pin
                           LED_YELLOW2_Pin LED_GREEN2_Pin LED_RED1_Pin */
	GPIO_InitStruct.Pin = LED_DEBUG_Pin|LED_YELLOW1_Pin|LED_GREEN1_Pin|LED_RED2_Pin
			|LED_YELLOW2_Pin|LED_GREEN2_Pin|LED_RED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : LED7SEG_a_Pin LED7SEG_b_Pin LED7SEG_c_Pin LED7SEG_d_Pin
                           LED7SEG_e_Pin LED7SEG_f_Pin LED7SEG_g_Pin */
	GPIO_InitStruct.Pin = LED7SEG_a_Pin|LED7SEG_b_Pin|LED7SEG_c_Pin|LED7SEG_d_Pin
			|LED7SEG_e_Pin|LED7SEG_f_Pin|LED7SEG_g_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void light_set1 (uint8_t n) {
	HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, !(n & (1<<2)));
	HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, !(n & (1<<1)));
	HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, !(n & (1<<0)));
}

void light_set2 (uint8_t n) {
	HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, !(n & (1<<2)));
	HAL_GPIO_WritePin(LED_YELLOW2_GPIO_Port, LED_YELLOW2_Pin, !(n & (1<<1)));
	HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, !(n & (1<<0)));
}

void display7SEG (int num) {
	HAL_GPIO_WritePin(LED7SEG_a_GPIO_Port, LED7SEG_a_Pin, !((~led7seg_map_of_number[num]) & (1 << 0)));
	HAL_GPIO_WritePin(LED7SEG_b_GPIO_Port, LED7SEG_b_Pin, !((~led7seg_map_of_number[num]) & (1 << 1)));
	HAL_GPIO_WritePin(LED7SEG_c_GPIO_Port, LED7SEG_c_Pin, !((~led7seg_map_of_number[num]) & (1 << 2)));
	HAL_GPIO_WritePin(LED7SEG_d_GPIO_Port, LED7SEG_d_Pin, !((~led7seg_map_of_number[num]) & (1 << 3)));
	HAL_GPIO_WritePin(LED7SEG_e_GPIO_Port, LED7SEG_e_Pin, !((~led7seg_map_of_number[num]) & (1 << 4)));
	HAL_GPIO_WritePin(LED7SEG_f_GPIO_Port, LED7SEG_f_Pin, !((~led7seg_map_of_number[num]) & (1 << 5)));
	HAL_GPIO_WritePin(LED7SEG_g_GPIO_Port, LED7SEG_g_Pin, !((~led7seg_map_of_number[num]) & (1 << 6)));
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
