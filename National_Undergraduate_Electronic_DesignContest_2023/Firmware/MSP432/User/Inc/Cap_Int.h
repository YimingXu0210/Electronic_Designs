#ifndef __CAP_INT_H
#define __CAP_INT_H

#include "config.h"

//输入捕获配置
#define CAP_TIMA_SELECTION TIMER_A1_BASE									//定时器
#define CAP_REGISTER_SELECTION TIMER_A_CAPTURECOMPARE_REGISTER_1			//定时器通道
#define CAP_SOURCE_DIV		TIMER_A_CLOCKSOURCE_DIVIDER_1					//时钟源分频
#define CAP_SOURCE_FREQ		(48000000u / CAP_SOURCE_DIV)
#define CAP_CCR_NUM 1														//定时器通道
#define CAP_PORT_PIN GPIO_PORT_P7, GPIO_PIN7								//复用引脚 P7.7
#define CAP_INTERRUPT INT_TA1_N												//中断号

//脉冲时基配置
#define INT_TIMA_SELECTION TIMER_A2_BASE									//定时器
#define INT_REGISTER_SELECTION TIMER_A_CAPTURECOMPARE_REGISTER_0			//定时器通道
#define INT_SOURCE_DIV		TIMER_A_CLOCKSOURCE_DIVIDER_48					//时钟源分频
#define INT_SOURCE_ARR		(50000 - 1)										//自动重装载值（ARR）
#define INT_FREQ_FX			(48000000u / INT_SOURCE_DIV / INT_SOURCE_ARR)	//频率倍率 Freq = EX_Cnt_In * INT_FREQ_FX
#define INT_INTERRUPT INT_TA2_0												//中断号

//脉冲计数配置
#define EX_TIMA_SELECTION TIMER_A3_BASE										//定时器
#define EX_REGISTER_SELECTION TIMER_A_CAPTURECOMPARE_REGISTER_0				//定时器通道
#define EX_PORT_PIN GPIO_PORT_P8, GPIO_PIN3									//复用引脚 P8.3
#define EX_INTERRUPT INT_TA3_0												//中断号

/*************************************************
 * Cap_erro = Freq_In / CAP_SOURCE_FREQ
 * Int_erro = 1 / (Freq_In / INT_FREQ_FX)
 * Freq_Mid = sqr(CAP_SOURCE_FREQ * INT_FREQ_FX)
*************************************************/

void Cap_Int_Init(void);

void Cap_Get_Start(uint32_t *Get_High_Cap, uint32_t *Get_Total_Cap, float *Get_Freq, float *Get_Duty);
void Int_Get_Start(uint32_t *Get_Freq);

#endif
