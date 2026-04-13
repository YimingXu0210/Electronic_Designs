#ifndef __ADC_IN_H
#define __ADC_IN_H

#include "config.h"

#define SIGNAL_SAMPLE_TIMER				TIMER_A0_BASE

#define TIMER_SOURCE_DIV				TIMER_A_CLOCKSOURCE_DIVIDER_1	// 定时器时钟源分频
#define TIMER_SOURCE_FREQ				(48000000u / TIMER_SOURCE_DIV)
#define SIGNAL_SAMPLE_FREQ_MAX			1000000u
#define ADC_MAX							16384u
#define ADC_RF_V_MV						2500u

#define ADC_SAMPLING_NUM				1024u							//2^Nu
#define SIGNAL_SAMPLE_FREQ_MULTIPLE		64u								// 采样频率设定为信号基波频率的几倍（Fs = ? F0）
#define SIGNAL_SAMPLE_PERIOD_MIN		(TIMER_SOURCE_FREQ / SIGNAL_SAMPLE_FREQ_MAX)
#define FX_VPP_MULTIPLE					1								// 计算的到的幅值乘以的倍数

extern uint16_t Signal_ADC_Data[ADC_SAMPLING_NUM];
extern __IO uint8_t DMA_Transmit_Completed_Flag;

void Adc_Init(void);
void Adc_Start(void);
void Adc_Fs_Change_F(uint32_t Freq);
void Adc_Fs_Change_C(uint32_t Captured_Value);
void Adc_Fs_Adjust_C(uint32_t Captured_Value);

#endif
