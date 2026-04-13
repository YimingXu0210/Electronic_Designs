#include "WS2812.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

uint16_t RGB_buffur[Reste_Data + Led_Num * Led_Data_Len];

void WS2812_SetColors_All(uint32_t colors)
{
	memset(RGB_buffur, 0, sizeof(RGB_buffur));
    uint16_t* p;
    uint32_t Color;

    for(uint8_t k = 0; k < Led_Num; k++)
    {
        p = (RGB_buffur + Reste_Data) + (k * Led_Data_Len);
        Color = colors;
		
        for (uint8_t i = 0; i < 8; ++i)
            p[i + 8] = (((Color << i) & 0X800000) ? Hight_Data : Low_Data);
        for (uint8_t i = 8; i < 16; ++i)
            p[i - 8] = (((Color << i) & 0X800000) ? Hight_Data : Low_Data);
        for (uint8_t i = 16; i < 24; ++i)
            p[i]       = (((Color << i) & 0X800000) ? Hight_Data : Low_Data);
    }
	
    // 启动DMA传输
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)RGB_buffur, (Reste_Data + Led_Num * Led_Data_Len));
}

void WS2812_SetColors(uint32_t* colors)
{
	memset(RGB_buffur, 0, sizeof(RGB_buffur));
    uint16_t* p;
    uint32_t Color;

    for(uint8_t k = 0; k < Led_Num; k++)
    {
        p = (RGB_buffur + Reste_Data) + (k * Led_Data_Len);
        Color = colors[k];
		
        for (uint8_t i = 0; i < 8; ++i)
            p[i + 8] = (((Color << i) & 0X800000) ? Hight_Data : Low_Data);
        for (uint8_t i = 8; i < 16; ++i)
            p[i - 8] = (((Color << i) & 0X800000) ? Hight_Data : Low_Data);
        for (uint8_t i = 16; i < 24; ++i)
            p[i]       = (((Color << i) & 0X800000) ? Hight_Data : Low_Data);
    }

	
//	// 打印复位数据
//	printf("Reset Data: ");
//	for (int i = 0; i < Reste_Data; i++) {
//		printf("%d ", RGB_buffur[i]);
//	}
//	printf("\n");

//	// 逐个LED打印RGB数据
//	for (int k = 0; k < Led_Num; k++) {
//		printf("LED %d: ", k + 1);
//		for (int i = 0; i < Led_Data_Len; i++) {
//			printf("%d ", RGB_buffur[Reste_Data + k * Led_Data_Len + i]);
//		}
//		printf("\n");
//	}

	
    // 启动DMA传输
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)RGB_buffur, (Reste_Data + Led_Num * Led_Data_Len));
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_1);
}
