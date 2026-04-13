#include "usart.h"

/* UART Configuration Parameter.
 * at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
 const eUSCI_UART_ConfigV1 uartConfig_9600_48Mhz =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        312,                                     // BRDIV
        8,                                       // UCxBRF
        0,                                       // UCxBRS
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                   // 8 bit data length
};
 
const eUSCI_UART_ConfigV1 uartConfig_115200_48Mhz =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        26,                                      // BRDIV
        0,                                       // UCxBRF
        111,                                     // UCxBRS
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                   // 8 bit data length
};

void USART_A0_Init(void)
{
	/* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig_115200_48Mhz);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    /* Enabling interrupts */
//    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
//    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
}

void USART_A1_Init(void)
{
	/* Selecting P2.2 and P2.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A1_BASE, &uartConfig_115200_48Mhz);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A1_BASE);

    /* Enabling interrupts */
    MAP_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA1);
}

void USART_A2_Init(void)
{
	/* Selecting P3.2 and P3.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig_115200_48Mhz);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A2_BASE);

    /* Enabling interrupts */
//    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
//    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
}

urx_buffer Uart_A0_Buf;
urx_buffer Uart_A1_Buf;


void Rx_A0_proc(void) {
	if(uwTick - Uart_A0_Buf.uwTick_Rx < 10) return;
	Uart_A0_Buf.uwTick_Rx = uwTick;

	if(Uart_A0_Buf.Size) {
//		printf("%s", Uart_A0_Buf.Buffer);
		
		memset(Uart_A0_Buf.Buffer, 0, Uart_A0_Buf.Size);
		Uart_A0_Buf.Size = 0;
	}
}

void EUSCIA0_IRQHandler(void) {
	if (MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE) & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
	{
		MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
		
		Uart_A0_Buf.Buffer[Uart_A0_Buf.Size++] = MAP_UART_receiveData(EUSCI_A0_BASE);
		Uart_A0_Buf.uwTick_Rx = uwTick;
		
		if(Uart_A0_Buf.Size >= BUFFER_SIZE) Uart_A0_Buf.uwTick_Rx = uwTick - 10;
	}
}

uint8_t __IO STM_STAT = 0;

void Rx_A1_proc(void) {
	if(uwTick - Uart_A1_Buf.uwTick_Rx < 10) return;
	Uart_A1_Buf.uwTick_Rx = uwTick;

	if(Uart_A1_Buf.Size) {
		printf_uart1("%s", Uart_A1_Buf.Buffer);
		
		if(Uart_A1_Buf.Size == 1) {
			if(Uart_A1_Buf.Buffer[0] == 0x01) {
				STM_STAT = 1;
				AFP_SK = 1;
			} else if(Uart_A1_Buf.Buffer[0] == 0x02) {
				STM_STAT = 1;
				AFP_SK = 2;
			} else if(Uart_A1_Buf.Buffer[0] == 0x03) {
				STM_STAT = 1;
				AFP_SK = 3;
			}
		}
		
		memset(Uart_A1_Buf.Buffer, 0, Uart_A1_Buf.Size);
		Uart_A1_Buf.Size = 0;
	}
}

void EUSCIA1_IRQHandler(void) {
	if (MAP_UART_getEnabledInterruptStatus(EUSCI_A1_BASE) & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
	{
		MAP_UART_clearInterruptFlag(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
		
		Uart_A1_Buf.Buffer[Uart_A1_Buf.Size++] = MAP_UART_receiveData(EUSCI_A1_BASE);
		Uart_A1_Buf.uwTick_Rx = uwTick;
		
		if(Uart_A1_Buf.Size >= BUFFER_SIZE) Uart_A1_Buf.uwTick_Rx = uwTick - 10;
	}
}


void EUSCIA2_IRQHandler(void) {
	if (MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE) & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
	{
		MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
		
		HMI_RX.Buffer[HMI_RX.Size++] = MAP_UART_receiveData(EUSCI_A2_BASE);
		HMI_RX.uwTick_Rx = uwTick;
		
		if(HMI_RX.Size >= BUFFER_SIZE) HMI_RX.uwTick_Rx = uwTick - 10;
	}
}



//  1 使用标准C库
//  0 使用微库
#if 0
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
#else
int fgetc(FILE *f)
{
	while (EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG !=
		MAP_UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG));
	return UART_receiveData(EUSCI_A0_BASE);
}
#endif
int fputc(int ch, FILE *f)
{
	MAP_UART_transmitData(EUSCI_A0_BASE, ch & 0xFF);
	return ch;
}

uint8_t UART1_TX_BUF[UART1_SEND_LEN_MAX]; //发送缓冲,最大UART1_SEND_LEN_MAX字节
uint8_t UART2_TX_BUF[UART2_SEND_LEN_MAX]; //发送缓冲,最大UART2_SEND_LEN_MAX字节

// 串口1,printf 函数
// 确保一次发送数据不超过USART1_MAX_SEND_LEN字节
void printf_uart1(char *fmt, ...)
{
	uint16_t i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)UART1_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)UART1_TX_BUF); // 此次发送数据的长度
	for (j = 0; j < i; j++)					// 循环发送数据
	{
		MAP_UART_transmitData(EUSCI_A1_BASE, UART1_TX_BUF[j]);
	}
}

// 串口2,printf 函数
// 确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void HMI_printf(char *fmt, ...)
{
	uint16_t i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)UART2_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)UART2_TX_BUF); // 此次发送数据的长度
	for (j = 0; j < i; j++)					// 循环发送数据
	{
		MAP_UART_transmitData(EUSCI_A2_BASE, UART2_TX_BUF[j]);
	}
}
