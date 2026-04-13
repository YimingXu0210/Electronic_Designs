/*
 * @Author: m-RNA m-RNA@qq.com
 * @Date: 2022-07-13 00:38:44
 * @LastEditors: m-RNA m-RNA@qq.com
 * @LastEditTime: 2022-08-07 00:40:30
 * @FilePath: \MDK-ARMe:\RNA_2021_Game_A\MCU Project\BLL\Inc\my_math.h
 * @Description: 我的相关算法文件
 */
#ifndef __MY_MATH
#define __MY_MATH
#include "sys.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "windowfunction.h"
#include "adc.h"

#ifdef __MSP432P401R__
#if (ADC_SAMPLING_NUM <= 4096u)
#define Signal_Synthesizer_Wave_Length_MAX ADC_SAMPLING_NUM
#else
#define Signal_Synthesizer_Wave_Length_MAX 1024u
#endif
#elif defined STM32F429xx
#if (ADC_SAMPLING_NUM <= 1024u)
#define Signal_Synthesizer_Wave_Length_MAX ADC_SAMPLING_NUM
#else
#define Signal_Synthesizer_Wave_Length_MAX 640u
#endif
#else
#warning...
#endif

/* 找出最小值位置 */
uint16_t Min_Float(float Mag[], uint16_t len);

/* 找出最大值位置 */
uint16_t Max_Unsigned(float Mag[], uint16_t len);

/* 在一定范围内找出最大值位置 */
uint16_t Max_Float_WithinRange(float Data[], uint16_t Left, uint16_t Right);

/* 通过FFT 计算各个频率分量幅值 */
void CalculateAmplitude_By_FFT(float *Am_Pointer, uint16_t *SampleData_Pointer);

/*  计算各个谐波分量幅值相位 */
void NormalizedAm_And_CalculateTHD(float *Phase_Pointer, float *NormAm_Pointer, uint16_t *Fx_Vpp_Pointer, float *THD_Pointer, float *Am_Data_Pointer);

/* 用归一化幅值+各分量相位 还原波形 */
void Restore_Waveform(uint16_t *RestoreWaveform_Pointer, float *NormAm_Pointer, float *Phase_Pointer);

/* 用幅值+各分量相位 还原波形 */
void Restore_Waveform_By_Vpp(uint16_t *RestoreWaveform, uint16_t *Fx_Vpp, float *Phase);

/**
 * @brief  信号合成器
 * @param[out] Output    波形数据输出指针
 * @param[in]  Length    波形数据输出长度
 * @param[in]  F0_Vpp    基波幅值(mv)
 * @param[in]  NormAm    归一化幅值
 * @param[in]  Phase     相位
 * @param[in]  Precision 最高几次谐波分量
 */
void Signal_Synthesizer(uint16_t *Output, uint16_t Length, uint16_t F0_Vpp, float *NormAm, float *Phase, uint8_t Precision);

/**
 * @brief  信号合成器
 * @param[out] Output    波形数据输出指针
 * @param[in]  Length    波形数据输出长度
 * @param[in]  Fx_Vpp    基波-谐波幅值(mv)
 * @param[in]  Phase     相位(弧度)
 * @param[in]  Precision 最高几次谐波分量
 */
void Signal_Synthesizer_Vpp(uint16_t *Output, uint16_t Length, uint16_t *Fx_Vpp, float *Phase, uint8_t Precision);

#endif
