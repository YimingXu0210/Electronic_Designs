#include "Cap_Int.h"

void Tim_Cap_Init(void) {
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(CAP_PORT_PIN, GPIO_PRIMARY_MODULE_FUNCTION);

	/* 定时器配置参数*/
	Timer_A_ContinuousModeConfig continuousModeConfig;

	continuousModeConfig.clockSource				=	TIMER_A_CLOCKSOURCE_SMCLK;       // SMCLK Clock Source
	continuousModeConfig.clockSourceDivider			=	CAP_SOURCE_DIV;					 // SMCLK/1 = 48MHz
	continuousModeConfig.timerInterruptEnable_TAIE	=	TIMER_A_TAIE_INTERRUPT_ENABLE;   // 开启定时器溢出中断
	continuousModeConfig.timerClear					=	TIMER_A_DO_CLEAR;                // Clear Counter

	//将定时器初始化为连续计数模式
	MAP_Timer_A_configureContinuousMode(CAP_TIMA_SELECTION, &continuousModeConfig);

	//配置捕捉模式结构体 */
	Timer_A_CaptureModeConfig captureModeConfig_TA2;

	captureModeConfig_TA2.captureRegister			=	CAP_REGISTER_SELECTION;                      //在这里改引脚
	captureModeConfig_TA2.captureMode				=	TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE; //上升下降沿捕获
	captureModeConfig_TA2.captureInputSelect		=	TIMER_A_CAPTURE_INPUTSELECT_CCIxA;           //CCIxA:外部引脚输入  （CCIxB:与内部ACLK连接(手册)
	captureModeConfig_TA2.synchronizeCaptureSource	=	TIMER_A_CAPTURE_SYNCHRONOUS;                 //同步捕获
	captureModeConfig_TA2.captureInterruptEnable	=	TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;     //开启CCRN捕获中断
	captureModeConfig_TA2.captureOutputMode			=	TIMER_A_OUTPUTMODE_OUTBITVALUE;              //输出位值

	//初始化定时器的捕获模式
	MAP_Timer_A_initCapture(CAP_TIMA_SELECTION, &captureModeConfig_TA2);

	//选择连续模式计数开始计数
//	MAP_Timer_A_startCounter(CAP_TIMA_SELECTION, TIMER_A_CONTINUOUS_MODE);

	//清除中断标志位
	MAP_Timer_A_clearInterruptFlag(CAP_TIMA_SELECTION);                                   //清除定时器溢出中断标志位
	MAP_Timer_A_clearCaptureCompareInterrupt(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION); //清除 CCR1 更新中断标志位

	//开启定时器端口中断
	MAP_Interrupt_enableInterrupt(CAP_INTERRUPT); //开启定时器A2端口中断
}

void Tim_Int_Init(void) {
	//增计数模式初始化
	Timer_A_UpModeConfig upConfig;
	
	upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;															//时钟源
	upConfig.clockSourceDivider = INT_SOURCE_DIV;																//时钟分频 范围1-64
	upConfig.timerPeriod = INT_SOURCE_ARR-1;																	//自动重装载值（ARR）
	upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;										//禁用 tim溢出中断
	upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;						//启用 ccr0更新中断
	upConfig.timerClear = TIMER_A_DO_CLEAR;																		//Clear value

	//初始化定时器A
	MAP_Timer_A_configureUpMode(INT_TIMA_SELECTION, &upConfig);

	//选择模式开始计数
//	MAP_Timer_A_startCounter(INT_TIMA_SELECTION, TIMER_A_UP_MODE);

	//清除比较中断标志位
	MAP_Timer_A_clearCaptureCompareInterrupt(INT_TIMA_SELECTION, INT_REGISTER_SELECTION);

	//开启串口端口中断
	MAP_Interrupt_enableInterrupt(INT_INTERRUPT);
}

void Tim_Ex_Init(void) {
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(EX_PORT_PIN, GPIO_PRIMARY_MODULE_FUNCTION);					//外部脉冲
	
	//增计数模式初始化
	Timer_A_UpModeConfig upConfig;
	
	upConfig.clockSource = TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK;													//时钟源(外部时钟源)
	upConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;												//时钟分频 范围1-64
	upConfig.timerPeriod = 65535;																				//自动重装载值（ARR）
	upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;										//禁用 tim溢出中断
	upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;						//启用 ccr0更新中断
	upConfig.timerClear = TIMER_A_DO_CLEAR;																		//Clear value

	//初始化定时器A
	MAP_Timer_A_configureUpMode(EX_TIMA_SELECTION, &upConfig);

	//选择模式开始计数
//	MAP_Timer_A_startCounter(EX_TIMA_SELECTION, TIMER_A_UP_MODE);

	//清除比较中断标志位
	MAP_Timer_A_clearCaptureCompareInterrupt(EX_TIMA_SELECTION, EX_REGISTER_SELECTION);

	//开启串口端口中断
	MAP_Interrupt_enableInterrupt(EX_INTERRUPT);
}

void Cap_Int_Init(void) {
	Tim_Cap_Init();
	Tim_Int_Init();
	Tim_Ex_Init();
}

uint32_t High_Cap;
uint32_t Total_Cap;
__IO uint8_t Cap_Get_Flag;

void TA1_N_IRQHandler(void)
{
	static uint8_t STAT;
	static uint16_t Capture_Buf[3];
	static uint16_t Overload_Cnt;


	if(MAP_Timer_A_getEnabledInterruptStatus(CAP_TIMA_SELECTION)) //溢出中断
	{
		MAP_Timer_A_clearInterruptFlag(CAP_TIMA_SELECTION); //清除定时器溢出中断标志位
		BITBAND_PERI(TIMER_A_CMSIS(CAP_TIMA_SELECTION)->CCTL[CAP_CCR_NUM], TIMER_A_CCTLN_COV_OFS) = 0;
		
		Overload_Cnt++;
	}

	if(MAP_Timer_A_getCaptureCompareEnabledInterruptStatus(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION)) //捕获中断
	{
		MAP_Timer_A_clearCaptureCompareInterrupt(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION); //清除 CCR1 更新中断标志位
		
		switch(STAT) {
			case 0:	//同步用
				if(MAP_Timer_A_getSynchronizedCaptureCompareInput(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION,
							TIMER_A_READ_CAPTURE_COMPARE_INPUT) == TIMER_A_CAPTURECOMPARE_INPUT_LOW) {	//下降沿
					STAT++;
				}
			break;
				
			case 1:
				if(MAP_Timer_A_getSynchronizedCaptureCompareInput(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION,
							TIMER_A_READ_CAPTURE_COMPARE_INPUT) == TIMER_A_CAPTURECOMPARE_INPUT_HIGH) {	//上升沿
					Capture_Buf[0] = TIMER_A_CMSIS(CAP_TIMA_SELECTION)->CCR[CAP_CCR_NUM];
					STAT++;
					Overload_Cnt = 0;
				}
			break;
				
			case 2:
				if(MAP_Timer_A_getSynchronizedCaptureCompareInput(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION,
							TIMER_A_READ_CAPTURE_COMPARE_INPUT) == TIMER_A_CAPTURECOMPARE_INPUT_LOW) {	//下降沿
					Capture_Buf[1] = TIMER_A_CMSIS(CAP_TIMA_SELECTION)->CCR[CAP_CCR_NUM];
					STAT++;
					
					High_Cap = (Overload_Cnt << 16) + Capture_Buf[1] - Capture_Buf[0];  //高电平时间
//					printf("%d %d %d ", Overload_Cnt, Capture_Buf[0], Capture_Buf[1]);
					Overload_Cnt = 0;
				}
			break;
				
			case 3:
				if(MAP_Timer_A_getSynchronizedCaptureCompareInput(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION,
							TIMER_A_READ_CAPTURE_COMPARE_INPUT) == TIMER_A_CAPTURECOMPARE_INPUT_HIGH) {	//上升沿
					
					MAP_Timer_A_stopTimer(CAP_TIMA_SELECTION);
					STAT = 0;
					Capture_Buf[2] = TIMER_A_CMSIS(CAP_TIMA_SELECTION)->CCR[CAP_CCR_NUM];
					Total_Cap = High_Cap + (Overload_Cnt << 16) + Capture_Buf[2] - Capture_Buf[1];  //总时间
//					printf("%d %d\r\n", Overload_Cnt, Capture_Buf[2]);					
					
					Cap_Get_Flag = 1;

//					float Freq_temp = 48000000.f / Total_Cap;
//					float Duty_temp = High_Cap*1.f / Total_Cap * 100.f;
//					printf("%.2f %.2f%%\r\n",Freq_temp, Duty_temp);
				}
			break;
			
			default:
				STAT = 0;
			break;
		}
		
	}
	
}

void Cap_Get_Start(uint32_t *Get_High_Cap, uint32_t *Get_Total_Cap, float *Get_Freq, float *Get_Duty) {
	MAP_Timer_A_startCounter(CAP_TIMA_SELECTION, TIMER_A_CONTINUOUS_MODE);
	Cap_Get_Flag = 0;
	
	uint8_t ccnnt = 0;
	while(!Cap_Get_Flag) {

	}
	
	*Get_High_Cap = High_Cap;
	*Get_Total_Cap = Total_Cap;
	*Get_Freq = CAP_SOURCE_FREQ*1.f / Total_Cap;
	*Get_Duty = High_Cap*1.f / Total_Cap * 100.f;
}


uint16_t EX_Overload_Cnt;
uint32_t EX_Cnt_In;
__IO uint8_t EX_Get_Flag;

void TA2_0_IRQHandler(void) {
	MAP_Timer_A_clearCaptureCompareInterrupt(INT_TIMA_SELECTION, INT_REGISTER_SELECTION);
	
	MAP_Timer_A_stopTimer(INT_TIMA_SELECTION);
	MAP_Timer_A_stopTimer(EX_TIMA_SELECTION);
	
	EX_Cnt_In = (EX_Overload_Cnt << 16) + TIMER_A_CMSIS(EX_TIMA_SELECTION)->R;
	EX_Get_Flag = 1;
}

void TA3_0_IRQHandler(void) {
	MAP_Timer_A_clearCaptureCompareInterrupt(EX_TIMA_SELECTION, EX_REGISTER_SELECTION);
	
	EX_Overload_Cnt++;
}

void Int_Get_Start(uint32_t *Get_Freq) {
	MAP_Timer_A_clearTimer(INT_TIMA_SELECTION);
	MAP_Timer_A_startCounter(INT_TIMA_SELECTION, TIMER_A_UP_MODE);
	MAP_Timer_A_clearTimer(EX_TIMA_SELECTION);
	MAP_Timer_A_startCounter(EX_TIMA_SELECTION, TIMER_A_UP_MODE);
	EX_Get_Flag = 0;
	EX_Overload_Cnt = 0;
	EX_Cnt_In = 0;
	
	while(!EX_Get_Flag);
	
	*Get_Freq = EX_Cnt_In * INT_FREQ_FX;
}
