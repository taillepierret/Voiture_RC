/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define offb_Pin GPIO_PIN_0
#define offb_GPIO_Port GPIOH
#define off_Pin GPIO_PIN_1
#define off_GPIO_Port GPIOH
#define onb_Pin GPIO_PIN_0
#define onb_GPIO_Port GPIOA
#define on_Pin GPIO_PIN_1
#define on_GPIO_Port GPIOA
#define cmd_pleins_phares_Pin GPIO_PIN_10
#define cmd_pleins_phares_GPIO_Port GPIOB
#define led_v_Pin GPIO_PIN_13
#define led_v_GPIO_Port GPIOB
#define direction_Pin GPIO_PIN_14
#define direction_GPIO_Port GPIOB
#define led_r_Pin GPIO_PIN_8
#define led_r_GPIO_Port GPIOA
#define CSN_Pin GPIO_PIN_15
#define CSN_GPIO_Port GPIOA
#define IRQ_Pin GPIO_PIN_8
#define IRQ_GPIO_Port GPIOB
#define CE_Pin GPIO_PIN_9
#define CE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
