/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define OFF_B_EN_Pin GPIO_PIN_0
#define OFF_B_EN_GPIO_Port GPIOH
#define OFF_EN_Pin GPIO_PIN_1
#define OFF_EN_GPIO_Port GPIOH
#define ON_B_EN_Pin GPIO_PIN_0
#define ON_B_EN_GPIO_Port GPIOA
#define ON_EN_Pin GPIO_PIN_1
#define ON_EN_GPIO_Port GPIOA
#define COURANT_MOTEUR_EN_Pin GPIO_PIN_4
#define COURANT_MOTEUR_EN_GPIO_Port GPIOA
#define TENSION_BATTERIE_EN_Pin GPIO_PIN_1
#define TENSION_BATTERIE_EN_GPIO_Port GPIOB
#define CMD_PLEINS_PHARES_EN_Pin GPIO_PIN_10
#define CMD_PLEINS_PHARES_EN_GPIO_Port GPIOB
#define LED_V_EN_Pin GPIO_PIN_13
#define LED_V_EN_GPIO_Port GPIOB
#define DIRECTION_EN_Pin GPIO_PIN_14
#define DIRECTION_EN_GPIO_Port GPIOB
#define LED_R_EN_Pin GPIO_PIN_8
#define LED_R_EN_GPIO_Port GPIOA
#define CSN_EN_Pin GPIO_PIN_15
#define CSN_EN_GPIO_Port GPIOA
#define SCK_EN_Pin GPIO_PIN_3
#define SCK_EN_GPIO_Port GPIOB
#define MISO_EN_Pin GPIO_PIN_4
#define MISO_EN_GPIO_Port GPIOB
#define MOSI_EN_Pin GPIO_PIN_5
#define MOSI_EN_GPIO_Port GPIOB
#define CE_EN_Pin GPIO_PIN_9
#define CE_EN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
