#include "Key.h"

static uint8_t Key_Scan(void) {
	if(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == 0) return 1;
	if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0) return 2;
	if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 0) return 3;
	if(HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) == 1) return 4;
	return 0;
}

void Key_Proc(void) {
	static uint32_t uwTick_Key = 0;
	static uint8_t Key_Old = 0;
	static uint8_t Key_Times = 0;
	
	if((uwTick - uwTick_Key) < 50) return;
	uwTick_Key = uwTick;
	
	Key_Times++;
	
	uint8_t Key_Val = Key_Scan();
	uint8_t Key_Down = Key_Val & (Key_Val ^ Key_Old);
	uint8_t Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
	
	if(Key_Val == 0 && Key_Old == 0) {
		Key_Times = 0;
		return;
	}
	
	Key_Old = Key_Val;
	
	if(Key_Down) printf("%d ", Key_Down);
	if(Key_Up) printf("%d %d\r\n", Key_Up, Key_Times);
	
}