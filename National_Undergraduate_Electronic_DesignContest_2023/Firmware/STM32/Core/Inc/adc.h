/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "tim.h"
#include "delay.h"
#include "string.h"
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */

#define SIGNAL_SAMPLE_TIMER				TIMER_A0_BASE

#define TIMER_SOURCE_DIV				2								// 定时器时钟源分频
#define TIMER_SOURCE_FREQ				(144000000u / TIMER_SOURCE_DIV)
#define SIGNAL_SAMPLE_FREQ_MAX			1000000u
#define ADC_MAX							4096u
#define ADC_RF_V_MV						3300u

#define ADC_SAMPLING_NUM				1024u							// 2^Nu
#define FFT_CALCULATE_NUM				4096u
#define SIGNAL_SAMPLE_FREQ_MULTIPLE		64u								// 采样频率设定为信号基波频率的几倍（Fs = ? F0）
#define SIGNAL_SAMPLE_PERIOD_MIN		(TIMER_SOURCE_FREQ / SIGNAL_SAMPLE_FREQ_MAX)
#define FX_VPP_MULTIPLE					10								// 计算的到的幅值乘以的倍数

extern uint16_t Signal_ADC_Data[ADC_SAMPLING_NUM];
extern __IO uint8_t DMA_Transmit_Completed_Flag;

void Adc_Start(void);
void Adc_Fs_Change_F(uint32_t Freq);
void Adc_Fs_Change_C(uint32_t Captured_Value);
void Adc_Fs_Adjust_C(uint32_t Captured_Value);


/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

