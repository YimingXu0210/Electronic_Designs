#include "TLV5618.h"
#include "delay.h"


/******************************************************************************
 * Use with SPI Slave Data Echo code example.
 *
 *                MSP432P401
 *              -----------------
 *             |                 |
 *             |                 |
 *             |                 |
 *             |             P1.6|-> Data Out (UCB0SIMO)
 *             |                 |
 *             |             P1.7|<- Data In (UCB0SOMI)
 *             |                 |
 *             |             P1.5|-> Serial Clock Out (UCB0CLK)
 *******************************************************************************/

/*
 *P1.6 - DI
 *P1.5 - SCK
 *P4.5 - CS
 */

//![Simple SPI Config]
/* SPI Master Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        48000000,                                   // SMCLK = DCO = 3MHZ
        500000,                                    // SPICLK = 500khz
        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
};
//![Simple SPI Config]

static void TCL5618_SPI_pin_init(void);
void TCL5618_init(void);
void write_data_to_TCL5618(uint16_t data);


static void TCL5618_SPI_pin_init(void)
{         

	/* Selecting P1.5 P1.6 and P1.7 in SPI mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring SPI in 3wire master mode */
    SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);
    /* Enable SPI module */
    SPI_enableModule(EUSCI_B0_BASE);

	
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN5);
	
}  



void TCL5618_init(void)
{        
	//SPI通信相关引脚配置
	TCL5618_SPI_pin_init();
}


void write_data_to_TCL5618(uint16_t  data)
{	
	uint16_t da_value = data;

	CS_L();
	delay_us(5);
	SPI_transmitData(EUSCI_B0_BASE,(da_value>>8)&0XFF);
	while(SPI_isBusy(EUSCI_B0_BASE));//等待发送结束 
	SPI_transmitData(EUSCI_B0_BASE,da_value&0XFF);
	while(SPI_isBusy(EUSCI_B0_BASE));//等待发送结束 
	delay_us(5);
	CS_H();

}

/*****************************************************************************
函数功能：TLC5618 DAC基本驱动函数
	1.sclk的上升沿数据进入输入寄存器
	2.在sclk为低的时候，CS的上升沿数据开始DA转化
	3.连续输入16位数据，高位在前，其中前4位为参数设置
*****************************************************************************/

void DAC5618_translate_a(uint16_t n)	//A通道转换函数
{
	uint16_t data_temp;
	data_temp=n;
	data_temp=data_temp|0x8000;							//对数据求或，确定通道A 0X8000。 如通道B输出，修改为0x0000
	write_data_to_TCL5618(data_temp);														//CS上升沿，数据开始转换	
}



void DAC5618_translate_b(uint16_t n)	//B通道转换函数
{
	uint16_t data_temp;
	data_temp=n;
	data_temp=data_temp|0x0000;							//对数据求或，确定通道A 0X8000。 如通道B输出，修改为0x0000
	write_data_to_TCL5618(data_temp);														//CS上升沿，数据开始转换	
}













