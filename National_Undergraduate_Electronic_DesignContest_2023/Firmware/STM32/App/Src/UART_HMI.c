#include "UART_HMI.h"

urx_buffer HMI_RX;
uint8_t start_flag=0,pwm_freq=0;
extern uint8_t pwm_flag;

static void Clear_Buffer(void) {
	memset(HMI_RX.Buffer, 0, HMI_RX.Size);
	HMI_RX.Flag = 0;
	HMI_RX.Size = 0;
}

/*-------------------------------------------------*/
/*函数名：串口3 printf函数                          */
/*参  数：char* fmt,...  格式化输出字符串和参数      */
/*返回值：无                                        */
/*-------------------------------------------------*/

__align(8) char USART2_TxBuff[USART_TXBUFF_SIZE];  

void HMI_printf(char* fmt, ...) 
{  
	unsigned int i, length;
	
	va_list ap;
	va_start(ap, fmt);
	vsprintf(USART2_TxBuff, fmt, ap);
	va_end(ap);	
	
	length=strlen((const char*)USART2_TxBuff);		
	while((USART2->SR&0X40) == 0);
	for(i = 0; i < length; i++)
	{			
		USART2->DR = USART2_TxBuff[i];
		while((USART2->SR&0X40) == 0);	
	}	
}

void HMI_RX_proc(void) {
	if(HMI_RX.Flag) {
		
//	HMI_printf("%s", HMI_RX.Buffer);
		
        if(HMI_RX.Buffer[0]==0x01 && HMI_RX.Size==1)
            start_flag = 1;
        else if(HMI_RX.Buffer[0]==0x00 && HMI_RX.Size==1)
        {
            pwm_freq = 1;
            pwm_flag =1;
        }
        else if(HMI_RX.Buffer[0]==0x06 && HMI_RX.Size==1)
        {
            printf("{malv}6Kbps\r\n");
            pwm_freq = 2;
            start_flag = 1;
        }
        else if(HMI_RX.Buffer[0]==0x08 && HMI_RX.Size==1)
        {
            printf("{malv}8Kbps\r\n");
            pwm_freq = 3;
            start_flag = 1;
        }
        else if(HMI_RX.Buffer[0]==0x0A && HMI_RX.Size==1)
        {
            printf("{malv}10Kbps\r\n");
            pwm_freq = 4;
            start_flag = 1;
        }
        
		Clear_Buffer();
	}
}
