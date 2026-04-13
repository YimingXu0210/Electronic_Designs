#ifndef __USART_H
#define __USART_H

#include "config.h"

#define UART1_SEND_LEN_MAX 128 // uart1 最大发送缓存字节数
#define UART2_SEND_LEN_MAX 128 // uart2 最大发送缓存字节数

extern uint8_t UART1_TX_BUF[UART1_SEND_LEN_MAX]; //发送缓冲,最大UART1_SEND_LEN_MAX字节
extern uint8_t UART2_TX_BUF[UART2_SEND_LEN_MAX]; //发送缓冲,最大UART2_SEND_LEN_MAX字节

void printf_uart1(char *fmt, ...);
void HMI_printf(char *fmt, ...);

void USART_A0_Init(void);
void USART_A1_Init(void);
void USART_A2_Init(void);

extern urx_buffer Uart_A0_Buf;
extern urx_buffer Uart_A1_Buf;

extern __IO uint8_t STM_STAT;

void Rx_A0_proc(void);
void Rx_A1_proc(void);

#endif
