/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
typedef enum{
	STATE_OFF,
	STATE_BLINKING,
	STATE_PWD,
	STATE_RANDOM,
	STATE_CALC_ADD,
	STATE_CALC_SUB
}State_t;

volatile State_t current_state = STATE_OFF;
volatile uint8_t virtual_btn_next = 0; // Flag for next mode button
volatile uint8_t virtual_btn_calc = 0; // Flag for calculator mode button
volatile int8_t calc_num1 = 60;		   // First number
volatile int8_t calc_num2 = -70;	   // Second number
volatile int8_t calc_result = 0;	   // Result of calculation
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void State_Machine(void);
void State_Off(void);
void State_Blinking(void);
void State_PWD(void);
void State_Random(void);
void State_Calc_Add(void);
void State_Calc_Sub(void);
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
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  // Start PWM for all 4 channels
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  State_Machine();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 160;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// set all LED
void Set_LEDs(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4)
{
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, c1);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, c2);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, c3);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, c4);
}

// state machine
void State_Machine(void)
{
	switch (current_state)
	{
	case STATE_OFF:       
		State_Off();
		break;
	case STATE_BLINKING:  
		State_Blinking();
		break;
	case STATE_PWD:       
		State_PWD();
		break;
	case STATE_RANDOM:    
		State_Random();
		break;
	case STATE_CALC_ADD:  
		State_Calc_Add();
		break;
	case STATE_CALC_SUB:  
		State_Calc_Sub();
		break;
	}
}

void State_Off(void)
{
	// Turn off all LED
	Set_LEDs(0, 0, 0, 0);
	HAL_Delay(500);

	if (virtual_btn_next == 1)
	{
		virtual_btn_next = 0;
		current_state = STATE_BLINKING;
	}

	if (virtual_btn_calc == 1)
	{
		virtual_btn_calc = 0;
		current_state = STATE_CALC_ADD;
	}
}

void State_Blinking(void)
{
	// Blinking
	Set_LEDs(1000, 1000, 0, 0); // green ogange
	HAL_Delay(500);

	Set_LEDs(0, 0, 1000, 1000); // red blue
	HAL_Delay(500);

	if (virtual_btn_next == 1)
	{
		virtual_btn_next = 0;
		current_state = STATE_PWD;
	}
}

void State_PWD(void)
{
	// PWD
	for (int16_t i = 0; i <= 1000; i += 20) {
		
		Set_LEDs(i, i, i, i);
		HAL_Delay(20);
		if (virtual_btn_next == 1) break;
	}

	for (int16_t i = 1000; i >= 0; i -= 20) {
		
		Set_LEDs(i, i, i, i);
		HAL_Delay(20);
		if (virtual_btn_next == 1) break;
	}

	if (virtual_btn_next == 1)
	{
		virtual_btn_next = 0;
		current_state = STATE_RANDOM;
	}
}

void State_Random(void)
{
	// Random LED
	uint32_t random_led = rand() % 4;
	uint32_t random_bright = (rand() % 801) + 200;

	Set_LEDs(0, 0, 0, 0);

	Set_LEDs(
		(random_led == 0) ? random_bright : 0,
		(random_led == 1) ? random_bright : 0,
		(random_led == 2) ? random_bright : 0,
		(random_led == 3) ? random_bright : 0
	);

	HAL_Delay(250);

	if (virtual_btn_next == 1)
	{
		virtual_btn_next = 0;
		current_state = STATE_OFF;
	}
}

void State_Calc_Add(void)
{
	// turn off all LED at 1500 ms
	Set_LEDs(0, 0, 0, 0);
	HAL_Delay(1500);

	// calculate sum in 16 - bit to prevent overflow
	int16_t safe_result = (int16_t)calc_num1 + (int16_t)calc_num2;

	// check overflow
	if (safe_result > 127 || safe_result < -127)
	{
		// blink all LED 5 times
		for (uint8_t i = 0; i < 5; i++)
		{
			Set_LEDs(1000, 1000, 1000, 1000);
			HAL_Delay(200);

			Set_LEDs(0, 0, 0, 0);
			HAL_Delay(200);
		}
		HAL_Delay(1000);
		// auto-transition to subtraction on error
		virtual_btn_calc = 0;
		current_state = STATE_CALC_SUB;
	}
	else
	{
		calc_result = calc_num1 + calc_num2;
		int8_t equation_steps[3] = { calc_num1, calc_num2, calc_result };

		for (uint8_t step = 0; step < 3; step++)
		{
			int8_t raw_val = equation_steps[step];
			uint8_t abs_val = abs(raw_val);
			uint8_t display_val = abs_val;

			// set 8th bit if the number is negative
			if (raw_val < 0) display_val |= 0x80;
			
			uint8_t lower_nibble = display_val & 0x0F;
			uint8_t upper_nibble = (display_val >> 4) & 0x0F;

			// show upper nibble if number is > 15
			if (display_val > 15)
			{
				Set_LEDs((upper_nibble & 0x08) ? 1000 : 0, 
						 (upper_nibble & 0x04) ? 1000 : 0, 
						 (upper_nibble & 0x02) ? 1000 : 0, 
						 (upper_nibble & 0x01) ? 1000 : 0);
				HAL_Delay(3000);
				if (virtual_btn_calc == 1) break;

				Set_LEDs(0, 0, 0, 0);
				HAL_Delay(800);
			}

			// show lower nibble
			Set_LEDs((lower_nibble & 0x08) ? 1000 : 0, 
					 (lower_nibble & 0x04) ? 1000 : 0, 
					 (lower_nibble & 0x02) ? 1000 : 0, 
					 (lower_nibble & 0x01) ? 1000 : 0);
			HAL_Delay(3000);
			if (virtual_btn_calc == 1) break;

			// animation +
			if (step == 0)
			{
				for (uint8_t b = 0; b < 3; b++)
				{
					Set_LEDs(1000, 0, 1000, 0);
					HAL_Delay(300);

					Set_LEDs(0, 1000, 0, 1000);
					HAL_Delay(300);
				}
				Set_LEDs(0, 0, 0, 0);
				HAL_Delay(600);
			}
			// animation =
			else if (step == 1)
			{
				for (uint8_t b = 0; b < 3; b++)
				{
					Set_LEDs(1000, 1000, 1000, 1000);
					HAL_Delay(300);

					Set_LEDs(0, 0, 0, 0);
					HAL_Delay(300);
				}
				HAL_Delay(600);
			}
			else
			{
				Set_LEDs(0, 0, 0, 0);
				HAL_Delay(4000);
			}
			if (virtual_btn_calc == 1) break;
		}
	}

	if (virtual_btn_calc == 1)
	{
		virtual_btn_calc = 0;
		current_state = STATE_CALC_SUB;
	}
}

void State_Calc_Sub(void)
{
	// turn off all LED at 1500 ms
	Set_LEDs(0, 0, 0, 0);
	HAL_Delay(1500);

	// calculate sum in 16 - bit to prevent overflow
	int16_t safe_result = (int16_t)calc_num1 - (int16_t)calc_num2;

	// check overflow
	if (safe_result > 127 || safe_result < -127)
	{
		// blink all LED 5 times
		for (uint8_t i = 0; i < 5; i++)
		{
			Set_LEDs(1000, 1000, 1000, 1000);
			HAL_Delay(200);
			Set_LEDs(0, 0, 0, 0);
			HAL_Delay(200);
		}
		HAL_Delay(1000);
		// auto-transition to State_off
		virtual_btn_calc = 0;
		current_state = STATE_OFF;
	}
	else
	{
		calc_result = calc_num1 - calc_num2;
		int8_t equation_steps[3] = { calc_num1, calc_num2, calc_result };

		for (uint8_t step = 0; step < 3; step++)
		{
			int8_t raw_val = equation_steps[step];
			uint8_t abs_val = abs(raw_val);
			uint8_t display_val = abs_val;

			// Set 8th bit if the number is negative
			if (raw_val < 0) display_val |= 0x80;

			uint8_t lower_nibble = display_val & 0x0F;
			uint8_t upper_nibble = (display_val >> 4) & 0x0F;

			// show upper nibble if number is > 15
			if (display_val > 15)
			{
				Set_LEDs((upper_nibble & 0x08) ? 1000 : 0, 
						 (upper_nibble & 0x04) ? 1000 : 0, 
						 (upper_nibble & 0x02) ? 1000 : 0, 
						 (upper_nibble & 0x01) ? 1000 : 0);

				HAL_Delay(3000);
				if (virtual_btn_calc == 1) break;

				Set_LEDs(0, 0, 0, 0);
				HAL_Delay(800);
			}

			// show lower nibble
			Set_LEDs((lower_nibble & 0x08) ? 1000 : 0,
					 (lower_nibble & 0x04) ? 1000 : 0,
					 (lower_nibble & 0x02) ? 1000 : 0,
					 (lower_nibble & 0x01) ? 1000 : 0);
			HAL_Delay(3000);
			if (virtual_btn_calc == 1) break;

			// animation -
			if (step == 0)
			{
				for (uint8_t b = 0; b < 3; b++)
				{
					Set_LEDs(1000, 0, 1000, 0);
					HAL_Delay(300);
					Set_LEDs(0, 0, 0, 0);
					HAL_Delay(300);
				}
				HAL_Delay(600);
			}
			// animation =
			else if (step == 1)
			{
				for (uint8_t b = 0; b < 3; b++)
				{
					Set_LEDs(1000, 1000, 1000, 1000);
					HAL_Delay(300);

					Set_LEDs(0, 0, 0, 0);
					HAL_Delay(300);
				}
				HAL_Delay(600);
			}
			else
			{
				Set_LEDs(0, 0, 0, 0);
				HAL_Delay(4000);
			}
			if (virtual_btn_calc == 1) break;
		}
	}

	if (virtual_btn_calc == 1)
	{
		virtual_btn_calc = 0;
		current_state = STATE_OFF;
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
