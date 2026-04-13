#ifndef __UART_HMI_H__
#define __UART_HMI_H__

#include "sys.h"
#include "main.h"

#include "usart.h"

#define USART_TXBUFF_SIZE   1024   		   //定义串口2 发送缓冲区大小 1024字节


extern uint8_t start_flag,pwm_freq;
extern urx_buffer HMI_RX;

void HMI_printf(char* fmt, ...);
void HMI_RX_proc(void);

#endif
