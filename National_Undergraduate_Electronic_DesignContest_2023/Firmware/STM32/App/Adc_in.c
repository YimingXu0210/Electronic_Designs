#include "Adc_in.h"

uint16_t Signal_ADC_Data[ADC_SAMPLING_NUM];
__IO uint8_t DMA_Transmit_Completed_Flag;
uint8_t OverSamplingFlag = 0;


/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 1024
#elif defined(__GNUC__)
__attribute__((aligned(1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[32];

// DMA初始化函数
void Dma_Cfg(void)
{
	/* Configuring DMA module */
	MAP_DMA_enableModule();
	MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);

    DMA_disableChannelAttribute(DMA_CH7_ADC14,
                                 UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
                                 UDMA_ATTR_HIGH_PRIORITY |
                                 UDMA_ATTR_REQMASK);

    /* Setting Control Indexes. In this case we will set the source of the
     * DMA transfer to ADC14 Memory 0
     *  and the destination to the
     * destination data array. */
    MAP_DMA_setChannelControl(DMA_CH7_ADC14 | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
#if ADC_SAMPLING_NUM > 1024
    MAP_DMA_setChannelControl(DMA_CH7_ADC14 | UDMA_ALT_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
#endif
    /* Assigning/Enabling Interrupts */
    MAP_DMA_assignInterrupt(DMA_INT1, 7);
    MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
    MAP_DMA_assignChannel(DMA_CH7_ADC14);
    MAP_DMA_clearInterruptFlag(7);
    MAP_Interrupt_enableMaster();

    /* Now that the DMA is primed and setup, enabling the channels. The ADC14
     * hardware should take over and transfer/receive all bytes */
//    MAP_DMA_enableChannel(7);
}

// TIMA初始化函数
void TimA0_Base_Cfg(uint16_t ccr0)
{
	const Timer_A_UpModeConfig upConfig = {
		TIMER_A_CLOCKSOURCE_SMCLK,           //时钟源
		TIMER_A_CLOCKSOURCE_DIVIDER_1,       //时钟分频 范围1-64
		ccr0-1,                              //自动重装载值（ARR）
		TIMER_A_TAIE_INTERRUPT_DISABLE,      //禁用 tim溢出中断
		TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, //禁用 ccr0更新中断
		TIMER_A_DO_CLEAR,                    // Clear Counter
	};
	const Timer_A_CompareModeConfig compareConfig = {
		TIMER_A_CAPTURECOMPARE_REGISTER_1,        // Use CCR1
		TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE, // Disable CCR interrupt
		TIMER_A_OUTPUTMODE_SET_RESET,             // Toggle output but
		ccr0/2,                                   // CCR
	};
	
	MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig); //选择定时器A1_TIMER_A0
	MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);
	
//	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
//		GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
//	MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE); //开启计数
}

// ADC初始化函数
void Adc_Cfg(void)
{
	/* Initializing ADC (MCLK/1/1) */
	MAP_ADC14_enableModule();                                                                 // 使能ADC14模块
	MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_NOROUTE); // 初始化ADC时钟 48MHz(超频?)

	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN6, GPIO_TERTIARY_MODULE_FUNCTION); // 模拟输入 P4.6
	MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);                                                    // 单通道配置 多次转化true
	
#define ADC_VREFPOS 1
#if ADC_VREFPOS == 1
	MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A7, false);     // 使用内部2.5V电压参考 非差分输入false
	MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V); // 使用内部2.5V电压参考
	MAP_REF_A_enableReferenceVoltage();            // 使能内部2.5V电压参考
#elif ADC_VREFPOS == 2
	MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A7, false);		// 使用VCC
#elif ADC_VREFPOS == 3
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN7 | GPIO_PIN6, GPIO_TERTIARY_MODULE_FUNCTION);	// P5.6 - VREF+  P5.7 - VREF-
	MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_EXTPOS_VREFNEG_EXTNEG, ADC_INPUT_A7, false);
#endif
	
	MAP_ADC14_enableInterrupt(ADC_INT0);                                                                    // ADC通道0的中断



	MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false); // 使用定时器A通道1作为触发源

	MAP_ADC14_enableConversion(); // 使能开始转换(触发后 自动ADC上电)
	delay_ms(1000);               // 等待稳定
}

void Adc_Init(void) {
	Adc_Cfg();
	Dma_Cfg();
	TimA0_Base_Cfg(TIMER_SOURCE_FREQ / 1000000);
}

void Adc_Start(void) {
#if ADC_SAMPLING_NUM <= 1024
	MAP_DMA_setChannelTransfer(DMA_CH7_ADC14 | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
			(void *)&ADC14->MEM[0], (void *)Signal_ADC_Data, ADC_SAMPLING_NUM);
#else
	MAP_DMA_setChannelTransfer(DMA_CH7_ADC14 | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG,
			(void *)&ADC14->MEM[0], (void *)Signal_ADC_Data, 1024);
	MAP_DMA_setChannelTransfer(DMA_CH7_ADC14 | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG,
			(void *)&ADC14->MEM[0], (void *)(Signal_ADC_Data+1024), 1024);
#endif

    MAP_DMA_enableChannel(7); // 使能7通道（ADC）

    DMA_Transmit_Completed_Flag = 0;      // 传输完成标志位清零
    MAP_Timer_A_startCounter(SIGNAL_SAMPLE_TIMER, TIMER_A_UP_MODE); // 开始计数 触发ADC定时采样
	
    while(!DMA_Transmit_Completed_Flag);  // 等待传输完成

}

void Adc_Fs_Change_F(uint32_t Freq) {
	uint32_t Signal_Fs_ARR = TIMER_SOURCE_FREQ / Freq;
	
	MAP_Timer_A_setCompareValue(SIGNAL_SAMPLE_TIMER, TIMER_A_CAPTURECOMPARE_REGISTER_0, Signal_Fs_ARR-1); // 调整Fs
}

void Adc_Fs_Change_C(uint32_t Captured_Value) {
	MAP_Timer_A_setCompareValue(SIGNAL_SAMPLE_TIMER, TIMER_A_CAPTURECOMPARE_REGISTER_0, Captured_Value-1); // 调整Fs
}

void Adc_Fs_Adjust_C(uint32_t Captured_Value)
{
    uint32_t Signal_Fs_ARR = 0;

    Signal_Fs_ARR = Captured_Value / SIGNAL_SAMPLE_FREQ_MULTIPLE;
	
    OverSamplingFlag = Captured_Value <= (SIGNAL_SAMPLE_PERIOD_MIN * SIGNAL_SAMPLE_FREQ_MULTIPLE);
    if (OverSamplingFlag)
    {
        Signal_Fs_ARR += Captured_Value;
    }
	
//	printf("{Signal_Fs_ARR}%d\r\n", Signal_Fs_ARR);
	
    MAP_Timer_A_setCompareValue(SIGNAL_SAMPLE_TIMER, TIMER_A_CAPTURECOMPARE_REGISTER_0, Signal_Fs_ARR-1); // 调整Fs
}

void DMA_INT1_IRQHandler(void)
{
#if ADC_SAMPLING_NUM <= 1024
	MAP_Timer_A_stopTimer(SIGNAL_SAMPLE_TIMER);
	DMA_Transmit_Completed_Flag = 1;
#else
	static uint8_t STAT = 0;
	STAT++;
	
	if(STAT == ADC_SAMPLING_NUM / 1024) {
		STAT = 0;
		MAP_Timer_A_stopTimer(SIGNAL_SAMPLE_TIMER);
		DMA_Transmit_Completed_Flag = 1;
		return;
	}
	
    /* Switch between primary and alternate bufferes with DMA's PingPong mode */
    if(DMA_getChannelAttribute(7) & UDMA_ATTR_ALTSELECT) {
		MAP_DMA_setChannelTransfer(DMA_CH7_ADC14 | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG,
				(void *)&ADC14->MEM[0], (void *)(Signal_ADC_Data+1024*(STAT+1)), 1024);
    }
    else {
		MAP_DMA_setChannelTransfer(DMA_CH7_ADC14 | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG,
				(void *)&ADC14->MEM[0], (void *)(Signal_ADC_Data+1024*(STAT+1)), 1024);
    }
#endif
}
