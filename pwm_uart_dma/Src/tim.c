/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

	LL_TIM_InitTypeDef tim;
	LL_TIM_OC_InitTypeDef oc;
	// inicializujeme struktury (abychom je nemuseli vyplĹovat celĂ©)
	LL_TIM_StructInit(&tim);
	LL_TIM_OC_StructInit(&oc);
	// povolĂ­me clock pro TIM2
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	tim.Prescaler = 800; // 8M/800 = 10kHz do timeru
	tim.Autoreload = 100; // strop timeru 100, f=100Hz (T=10ms)
	LL_TIM_Init(TIM2,&tim);

	oc.OCPolarity = LL_TIM_OCPOLARITY_HIGH; // polarita bÄ›ĹľnĂˇ
	oc.OCState = LL_TIM_OCSTATE_ENABLE; // pĹ™idÄ›lujeme timeru ovlĂˇdĂˇnĂ­ vĂ˝stupu
	oc.OCMode = LL_TIM_OCMODE_PWM1; // reĹľim PWM1
	oc.CompareValue = 0; // startovacĂ­ hodnota PWM pro CH3
	LL_TIM_OC_Init(TIM2,LL_TIM_CHANNEL_CH1,&oc); // aplikujeme nastavenĂ­ na CH1 (PA5)
	LL_TIM_OC_EnablePreload(TIM2,LL_TIM_CHANNEL_CH1);

	LL_TIM_EnableIT_UPDATE(TIM2); // povolĂ­m pĹ™eruĹˇenĂ­ od pĹ™eteÄŤenĂ­
	NVIC_SetPriority(TIM2_IRQn,2); // nĂ­zkĂˇ priorita
	NVIC_EnableIRQ(TIM2_IRQn);
	// a ÄŤĂ­taÄŤ spustĂ­me
	LL_TIM_EnableCounter(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    /**TIM2 GPIO Configuration
    PA5     ------> TIM2_CH1
    */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
