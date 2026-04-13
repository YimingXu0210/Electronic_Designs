#include "delay.h"

static uint8_t fac_us = 48;

void Delay_Init(void)
{
	fac_us = CS_getMCLK() / 1000000; //系统时钟
	MAP_SysTick_enableModule();
	MAP_SysTick_setPeriod(MAP_CS_getMCLK() / 1000);
}

void delay_ms(uint32_t nms)
{
	while (nms)
	{
		delay_us(1000);
		--nms;
	}
}

void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; // LOAD的值
	ticks = nus * fac_us;			 // 需要的节拍数
	tcnt = 0;
	told = SysTick->VAL; // 刚进入时的计数器值
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow; // 这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; // 时间超过/等于要延迟的时间,则退出.
		}
	}
}
