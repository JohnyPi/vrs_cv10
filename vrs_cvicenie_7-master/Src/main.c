/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include<ctype.h>
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */
uint8_t velke = 0;
uint8_t male = 0;
uint8_t state_ = 0;
uint8_t index_P = 0;

#define BUFF_LEN 50
char buff[BUFF_LEN];
char dat_[100];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void receive_dma_data(const uint8_t *data, uint16_t len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */


int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* System interrupt init*/

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	USART2_RegisterCallback(receive_dma_data);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		uint16_t get_buff = DMA_buffer_size();
		float buff_prtg = (float) (get_buff) / (float) (DMA_USART2_BUFFER_SIZE)*100;
		sprintf(dat_, "Buffer capacity: %d bytes, occupied memory: %d bytes, load [in %]:%.1f%%\r\n", DMA_USART2_BUFFER_SIZE, get_buff, buff_prtg);

		USART2_PutBuffer((uint8_t *) dat_, sizeof(dat_));
		LL_mDelay(1000);

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

	if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0) {
		Error_Handler();
	}
	LL_RCC_HSI_Enable();

	/* Wait till HSI is ready */
	while (LL_RCC_HSI_IsReady() != 1) {

	}
	LL_RCC_HSI_SetCalibTrimming(16);
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

	/* Wait till System clock is ready */
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) {

	}
	LL_Init1msTick(8000000);
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
	LL_SetSystemCoreClock(8000000);
}



/* USER CODE BEGIN 4 */
void receive_dma_data(const uint8_t *data, uint16_t len) {
	if (*(data) == 0x0D) {
		return;
	}
	uint8_t RX_len = len - 1;
	if (*(data) == '#') {
		state_ = 1;

		for (uint8_t i = 1; i < RX_len; i++) {

			if (*(data + i) == '$') {
				for (uint8_t j = 1; j < i; j++) {
					if (isupper(*(data + j))) {
						velke++;
						if (velke >= 255) {
							velke = 0;
						}
					} else if (islower(*(data + j))) {
						male++;
						if (male >= 255) {
							male = 0;
						}
					}

				}
				memset(buff, '\0', BUFF_LEN);
				state_ = 0;
			}

			else {
				char c = *(data + i);
				strncat(buff, &c, 1);

			}
		}
		index_P = strlen(buff) + 1;

	} 

	else {
		memset(buff, '\0', BUFF_LEN);
		return;
	}

}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
