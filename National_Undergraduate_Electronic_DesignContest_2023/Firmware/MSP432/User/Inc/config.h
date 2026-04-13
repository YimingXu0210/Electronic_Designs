#ifndef __CONFIG_H
#define __CONFIG_H

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//定义一些常用的数据类型短关键字 
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;  

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  
typedef __I int16_t vsc16; 
typedef __I int8_t vsc8;   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 
typedef __I uint8_t vuc8;

#define BUFFER_SIZE 256
typedef struct {
	uint8_t Buffer[BUFFER_SIZE];					// 串口接收一级缓存
	uint16_t Size;									// 接收长度
	uint8_t Flag;									// 接收完成标志
	uint32_t uwTick_Rx;
} urx_buffer;

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "arm_math.h"
#include "arm_const_structs.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "tim.h"
#include "UART_HMI.h"

#include "Led.h"
#include "Key.h"
#include "IO_control.h"
#include "eeprom.h"
#include "TLV5618.h"
#include "AD985x.h"
#include "ADF4351.h"

#include "Cap_Int.h"
#include "Adc_in.h"
#include "my_math.h"
#include "windowfunction.h"

#endif
