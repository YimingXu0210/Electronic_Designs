#include "tim.h"

//void Tim_A1_PWM_Init(void)
//{
//    //初始化引脚
//    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
//	
//	/* 定时器配置参数*/
//    Timer_A_PWMConfig TimA1_PWMConfig;
//    
//    TimA1_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;             //时钟源
//    TimA1_PWMConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_48; //时钟分频 范围1-64
//    TimA1_PWMConfig.timerPeriod = 1000-1;                   			 //自动重装载值（ARR）
//    TimA1_PWMConfig.compareRegister = 0; 								 //通道
//    TimA1_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;   //输出模式
//    TimA1_PWMConfig.dutyCycle = 300;                                   	 //CCR
//	//
//	
//	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
//    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);				 //初始化比较寄存器以产生 PWM1
//	
//	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
//    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);				 //初始化比较寄存器以产生 PWM2
//	
//	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
//    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);				 //初始化比较寄存器以产生 PWM3
//	
//	TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;
//    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);				 //初始化比较寄存器以产生 PWM4
//}
