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
#include "stm32g0xx_hal.h"

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
void SystemClock_Config(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ETH_RSTn_Pin GPIO_PIN_0
#define ETH_RSTn_GPIO_Port GPIOA
#define ETH_INTn_Pin GPIO_PIN_1
#define ETH_INTn_GPIO_Port GPIOA
#define ETH_SCSn_Pin GPIO_PIN_7
#define ETH_SCSn_GPIO_Port GPIOA
#define TEST_LED_Pin GPIO_PIN_1
#define TEST_LED_GPIO_Port GPIOB
#define LED_PWR_EN_Pin GPIO_PIN_15
#define LED_PWR_EN_GPIO_Port GPIOB
#define LED_DATA_Pin GPIO_PIN_8
#define LED_DATA_GPIO_Port GPIOA
#define LED_PWR_OK_Pin GPIO_PIN_9
#define LED_PWR_OK_GPIO_Port GPIOA
#define ESP_IO2_Pin GPIO_PIN_4
#define ESP_IO2_GPIO_Port GPIOB
#define ESP_IO0_Pin GPIO_PIN_5
#define ESP_IO0_GPIO_Port GPIOB
#define ESP_EN_Pin GPIO_PIN_8
#define ESP_EN_GPIO_Port GPIOB
#define ESP_RSTn_Pin GPIO_PIN_9
#define ESP_RSTn_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
