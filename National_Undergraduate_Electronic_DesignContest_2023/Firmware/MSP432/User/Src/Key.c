#include "Key.h"

__IO uint8_t Choose = 0;

void Key_Init(void) {
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
}

static uint8_t Key_Scan(void) {
	if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) return 1;	// P1.1 S1
	if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == 0) return 2;	// P1.4 S2
	return 0;
}

void Key_Proc(void) {
	static uint32_t uwTick_Proc = 0;
	static uint8_t Key_Old = 0;
	static uint8_t Key_Times = 0;
	
	if(uwTick - uwTick_Proc < 50) return;
	uwTick_Proc = uwTick;
	
	Key_Times++;
	
	uint8_t Key_Val = Key_Scan();
	uint8_t Key_Down = Key_Val & (Key_Val ^ Key_Old);
	uint8_t Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
	
	if(Key_Val == 0 && Key_Old == 0) {
		Key_Times = 0;
		return;
	}
	
	Key_Old = Key_Val;
	
	
	
	if(Key_Down) {
		Choose = Key_Down;
	}

//	
//	if(Key_Up) {
//		printf("%d %d\r\n", Key_Up, Key_Times);
//	}

}
