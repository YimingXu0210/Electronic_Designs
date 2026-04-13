#include "sys.h"

#include "delay.h"

__IO uint32_t uwTick;
uint32_t uwTickFreq = TICK_FREQ_DEFAULT;

void SystemClock_Config(void)
{
	MAP_WDT_A_holdTimer();

	/* 第一步需要配置我们的时钟引脚，这里的高速时钟使用的是外部晶振*/
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION); //High
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION); //Low
	MAP_CS_setExternalClockSourceFrequency(32768, 48000000);

	/* Starting HFXT in non-bypass mode without a timeout. Before we start
	 * we have to change VCORE to 1 to support the 48MHz frequency */
	MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
	MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
	MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);

	MAP_CS_startHFXT(false);          //这是晶体 需要驱动
	MAP_CS_startLFXT(CS_LFXT_DRIVE3); //驱动等级3

	MAP_CS_initClockSignal(CS_MCLK	, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);	//48MHz   16分频时，滴答延时可达到最长
	MAP_CS_initClockSignal(CS_SMCLK	, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);	//48MHz
	MAP_CS_initClockSignal(CS_ACLK	, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_BCLK	, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

	MAP_FPU_enableModule();
	MAP_FPU_enableLazyStacking();
	
	Delay_Init();
	
	MAP_SysTick_enableInterrupt();
}

void SysTick_Handler(void)
{
    uwTick += uwTickFreq;
}
