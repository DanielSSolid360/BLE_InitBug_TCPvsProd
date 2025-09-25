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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RL3_Pin GPIO_PIN_5
#define RL3_GPIO_Port GPIOA
#define RL4_Pin GPIO_PIN_6
#define RL4_GPIO_Port GPIOA
#define CT3_Pin GPIO_PIN_7
#define CT3_GPIO_Port GPIOA
#define CT4_Pin GPIO_PIN_0
#define CT4_GPIO_Port GPIOB
#define RL1_Pin GPIO_PIN_1
#define RL1_GPIO_Port GPIOB
#define RL2_Pin GPIO_PIN_2
#define RL2_GPIO_Port GPIOB
#define CT1_Pin GPIO_PIN_10
#define CT1_GPIO_Port GPIOB
#define CT2_Pin GPIO_PIN_11
#define CT2_GPIO_Port GPIOB
#define PULSADOR_Pin GPIO_PIN_13
#define PULSADOR_GPIO_Port GPIOB
#define RL7_Pin GPIO_PIN_15
#define RL7_GPIO_Port GPIOB
#define RL8_Pin GPIO_PIN_8
#define RL8_GPIO_Port GPIOA
#define CT7_Pin GPIO_PIN_9
#define CT7_GPIO_Port GPIOA
#define CT8_Pin GPIO_PIN_6
#define CT8_GPIO_Port GPIOC
#define RL5_Pin GPIO_PIN_7
#define RL5_GPIO_Port GPIOC
#define RL6_Pin GPIO_PIN_10
#define RL6_GPIO_Port GPIOA
#define CT5_Pin GPIO_PIN_11
#define CT5_GPIO_Port GPIOA
#define CT6_Pin GPIO_PIN_12
#define CT6_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOD
#define RST_ESP32_Pin GPIO_PIN_1
#define RST_ESP32_GPIO_Port GPIOD
#define CT10_Pin GPIO_PIN_3
#define CT10_GPIO_Port GPIOD
#define CT9_Pin GPIO_PIN_3
#define CT9_GPIO_Port GPIOB
#define RL10_Pin GPIO_PIN_4
#define RL10_GPIO_Port GPIOB
#define RL9_Pin GPIO_PIN_5
#define RL9_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
