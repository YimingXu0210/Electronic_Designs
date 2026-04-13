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
#include "stm32f4xx_hal.h"

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
#define MAX31856_DRDY_1_Pin GPIO_PIN_2
#define MAX31856_DRDY_1_GPIO_Port GPIOE
#define MAX31856_CS_1_Pin GPIO_PIN_3
#define MAX31856_CS_1_GPIO_Port GPIOE
#define MAX31856_FAULT_1_Pin GPIO_PIN_4
#define MAX31856_FAULT_1_GPIO_Port GPIOE
#define MAX31856_DRDY_2_Pin GPIO_PIN_5
#define MAX31856_DRDY_2_GPIO_Port GPIOE
#define MAX31856_CS_2_Pin GPIO_PIN_6
#define MAX31856_CS_2_GPIO_Port GPIOE
#define MAX31856_FAULT_2_Pin GPIO_PIN_11
#define MAX31856_FAULT_2_GPIO_Port GPIOI
#define MAX31856_DRDY_3_Pin GPIO_PIN_7
#define MAX31856_DRDY_3_GPIO_Port GPIOF
#define MAX31856_DRDY_4_Pin GPIO_PIN_8
#define MAX31856_DRDY_4_GPIO_Port GPIOF
#define MAX31856_CS_4_Pin GPIO_PIN_9
#define MAX31856_CS_4_GPIO_Port GPIOF
#define MAX31856_FAULT_4_Pin GPIO_PIN_1
#define MAX31856_FAULT_4_GPIO_Port GPIOC
#define MAX31856_FAULT_3_Pin GPIO_PIN_2
#define MAX31856_FAULT_3_GPIO_Port GPIOH
#define MAX31856_CS_3_Pin GPIO_PIN_3
#define MAX31856_CS_3_GPIO_Port GPIOH
#define MAX31856_DRDY_6_Pin GPIO_PIN_6
#define MAX31856_DRDY_6_GPIO_Port GPIOH
#define MAX31856_FAULT_5_Pin GPIO_PIN_7
#define MAX31856_FAULT_5_GPIO_Port GPIOH
#define MAX31856_FAULT_7_Pin GPIO_PIN_8
#define MAX31856_FAULT_7_GPIO_Port GPIOH
#define MAX31856_CS_5_Pin GPIO_PIN_9
#define MAX31856_CS_5_GPIO_Port GPIOH
#define MAX31856_DRDY_5_Pin GPIO_PIN_10
#define MAX31856_DRDY_5_GPIO_Port GPIOH
#define MAX31856_CS_7_Pin GPIO_PIN_12
#define MAX31856_CS_7_GPIO_Port GPIOB
#define MAX31856_DRDY_7_Pin GPIO_PIN_13
#define MAX31856_DRDY_7_GPIO_Port GPIOB
#define MAX31856_FAULT_6_Pin GPIO_PIN_14
#define MAX31856_FAULT_6_GPIO_Port GPIOB
#define MAX31856_CS_6_Pin GPIO_PIN_15
#define MAX31856_CS_6_GPIO_Port GPIOB
#define ADG16_A0_Pin GPIO_PIN_0
#define ADG16_A0_GPIO_Port GPIOI
#define ADG16_A1_Pin GPIO_PIN_1
#define ADG16_A1_GPIO_Port GPIOI
#define ADG16_A2_Pin GPIO_PIN_2
#define ADG16_A2_GPIO_Port GPIOI
#define ADG16_A3_Pin GPIO_PIN_3
#define ADG16_A3_GPIO_Port GPIOI
#define ADG16_EN_Pin GPIO_PIN_11
#define ADG16_EN_GPIO_Port GPIOG
#define MAX31856_DRDY_0_Pin GPIO_PIN_3
#define MAX31856_DRDY_0_GPIO_Port GPIOB
#define MAX31856_FAULT_0_Pin GPIO_PIN_4
#define MAX31856_FAULT_0_GPIO_Port GPIOB
#define MAX31856_CS_0_Pin GPIO_PIN_5
#define MAX31856_CS_0_GPIO_Port GPIOB
#define ADG_A0_Pin GPIO_PIN_4
#define ADG_A0_GPIO_Port GPIOI
#define ADG_A1_Pin GPIO_PIN_5
#define ADG_A1_GPIO_Port GPIOI
#define ADG_EN_Pin GPIO_PIN_6
#define ADG_EN_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
