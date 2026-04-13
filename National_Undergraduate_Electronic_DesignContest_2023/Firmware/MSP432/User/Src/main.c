/******************************************************************************
 * MSP432 Project
 *
 * Description: An project that uses DriverLib. In this project, DriverLib
 * is built from source instead of the usual library.
 *
 *                MSP432P401R
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: Sparkim
*******************************************************************************/
#include "config.h"

float Amplitude_Data[FFT_CALCULATE_NUM]; // 各个频率分量幅值(FFT后)	// 补0

uint32_t Get_Freq;

uint8_t AFC;			// CW:1 AM:2 FM:3
float M;				// 调制系数
uint32_t Freq;			// 调制频率
uint32_t Freq_Dev;		// 最大频偏


int main(void)
{
    SystemClock_Config();
	
	USART_A0_Init();
	USART_A1_Init();
	USART_A2_Init();

	Led_Init();
	Key_Init();
	SW_Init();			//P10.0 1 2 3 4 5
	
	Cap_Int_Init();
	Adc_Init();
	
//	printf("HELLO\r\n");
	
	Window_Function_Init(Hanning, ADC_SAMPLING_NUM); // Boxcar, Triang, Hanning, Hamming, Blackman, Flattop
	
//	delay_ms(1000);
	CW_OUT();			// 并告知stm32输出直流
	printf_uart1("\x00");
//	delay_ms(1000);
	
    while(1)
    {
		if(Choose == 0) {
			
			LED_RED_On();
			while(!Choose) Key_Proc();
			LED_RED_Off();
			
		}else if(Choose == 1) {
			
			LED_RED_Off();
			LED_W_Off();
			LED_R_On();
			
			MSP432_ADC();
			
			Identify_AFC(Amplitude_Data, &AFC, &M, &Freq, &Freq_Dev);
			
		} else if(Choose == 2) {
			
			LED_RED_Off();
			LED_W_Off();
			LED_G_On();
			
			STM32_ADC();
			
			Identify_AFP_SK(Amplitude_Data);
			
		}

//        delay_ms(500);
		
    }
	
}
