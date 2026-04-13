#ifndef __APP_UART_H
#define __APP_UART_H

#include "app_main.h"

#define _DEBUG_  1

#if _DEBUG_
#define ps(...)			printf(__VA_ARGS__)
#else
#define ps(...)
#endif

#define printf_TTL(...) 		Uart_printf(&huart8, __VA_ARGS__)
#define printf_485_1(...) 		Uart_printf(&huart3, __VA_ARGS__)
#define printf_485_2(...) 		Uart_printf(&huart5, __VA_ARGS__)
#define printf_485_3(...) 		Uart_printf(&huart6, __VA_ARGS__)

#define BUFFER_SIZE 1024
typedef struct {
	uint8_t Buffer[BUFFER_SIZE];			// 串口接收一级缓存
	uint8_t Size;							// 接收长度
	uint8_t Flag;							// 接收完成标志
} uart_struct_buf;

void Uart_printf(UART_HandleTypeDef *huart, char *format, ...);

#endif /* __APP_UART_H */
