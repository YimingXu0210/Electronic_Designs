#include "app_uart.h"

#include "usart.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

}

int fputc(int ch, FILE* f) {
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFF);
	return ch;
}

void Uart_printf(UART_HandleTypeDef *huart,char *fmt, ...){
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);
	HAL_UART_Transmit(huart, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
