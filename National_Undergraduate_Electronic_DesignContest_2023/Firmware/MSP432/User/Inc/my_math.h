#ifndef __MY_MATH
#define __MY_MATH
#include "config.h"

#ifdef __MSP432P401R__
#if (ADC_SAMPLING_NUM <= 4096u)
#define Signal_Synthesizer_Wave_Length_MAX ADC_SAMPLING_NUM
#else
#define Signal_Synthesizer_Wave_Length_MAX 1024u
#endif
#elif defined __STM32F1xx_HAL_H
#if (ADC_SAMPLING_NUM < 1024u)
#define Signal_Synthesizer_Wave_Length_MAX ADC_SAMPLING_NUM
#else
#define Signal_Synthesizer_Wave_Length_MAX 640u
#endif
#else
#warning...
#endif

#define MAX_PEAKS 100 // 最大峰值数量

typedef struct {
    int index;
    float value;
} Peak;

typedef struct {
    Peak peaks[MAX_PEAKS];
    int numPeaks;
} SpectrumInfo;

extern uint8_t AFP_SK;

void extractSpectrumInfo(float spectrum[], int spectrumSize, SpectrumInfo* spectrumInfo, float threshold);

void Identify_AFP_SK(float *Am_Pointer);

void Identify_AFC(float *Am_Pointer, uint8_t *AFC, float *M, uint32_t *Freq, uint32_t *Freq_Dev);

/* 找出最小值位置 */
uint16_t Min_Float(float Mag[], uint16_t len);

/* 找出最大值位置 */
uint16_t Max_Unsigned(uint16_t Mag[], uint16_t len);

/* 在一定范围内找出最大值位置 */
uint16_t Max_Float_WithinRange(float Data[], uint16_t Left, uint16_t Right);

/* 通过FFT 计算各个频率分量幅值 */
void CalculateAmplitude_By_FFT(float *Am_Pointer, uint16_t *SampleData_Pointer, uint8_t Remove_Avg_flag);

#endif
