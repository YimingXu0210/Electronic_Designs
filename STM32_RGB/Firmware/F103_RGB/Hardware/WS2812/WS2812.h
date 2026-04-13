#ifndef __WS2812_H
#define __WS2812_H
#include "main.h"
#define Hight_Data            ( 72  )                           //1 码相对计数值
#define Low_Data              ( 22  )                           //0 码相对计数值
#define Reste_Data            ( 250 )                           //80 复位电平相对计数值
#define Led_Num               ( 40  )                           //WS2812灯个数
#define Led_Data_Len          ( 24  )                           //WS2812数据长度，单个需要24个字节
#define WS2812_Data_Len   (Led_Num * Led_Data_Len)              //ws2812级联后需要的数组长度

void WS2812_SetColors(uint32_t* colors);
void WS2812_SetColors_All(uint32_t colors);
 
#endif
