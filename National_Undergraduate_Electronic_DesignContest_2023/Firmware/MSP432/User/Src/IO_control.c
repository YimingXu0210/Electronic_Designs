#include "IO_control.h"

void SW_Init(void) {
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN7);
	
	SW_4 = 0;
	SW_3 = 0;
	SW_2 = 0;
	SW_1 = 0;
	
	ADC_TO_MSP432 = 0;
	ADC_TO_STM32 = 0;
}

void MSP432_ADC(void) {
	ADC_TO_MSP432 = 1;
	ADC_TO_STM32 = 0;
	
	delay_ms(100);
}

void STM32_ADC(void) {
	ADC_TO_MSP432 = 0;
	ADC_TO_STM32 = 1;
	
	delay_ms(100);
}

#define	OUT_Delay 500

void AM_OUT(void) {
	SW_4 = 1;
	SW_3 = 1;
	SW_2 = 1;
	SW_1 = 0;
	
	delay_ms(OUT_Delay);
}

void FM_OUT(void) {
	SW_4 = 0;
	SW_3 = 1;
	SW_2 = 1;
	SW_1 = 0;
	
	delay_ms(OUT_Delay);
}

void CW_OUT(void) {
	SW_4 = 0;
	SW_3 = 0;
	SW_2 = 0;
	SW_1 = 1;
	
	delay_ms(OUT_Delay);
}

void ASK_OUT(void) {
	SW_4 = 0;
	SW_3 = 0;
	SW_2 = 0;
	SW_1 = 0;
	
	delay_ms(OUT_Delay);
}

void FSK_OUT(void) {
	SW_4 = 0;
	SW_3 = 0;
	SW_2 = 0;
	SW_1 = 0;
	
	delay_ms(OUT_Delay);
}

void PSK_OUT(void) {
	SW_4 = 0;
	SW_3 = 0;
	SW_2 = 0;
	SW_1 = 1;
	
	delay_ms(OUT_Delay);
}
