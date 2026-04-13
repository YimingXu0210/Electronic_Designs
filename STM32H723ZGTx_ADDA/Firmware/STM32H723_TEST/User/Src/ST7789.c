#include "ST7789.h"
#include "dma.h"
#include "spi.h"

static void LCD_WriteCommand(uint8_t cmd)
{
    LCD_WR_RS(0);
    HAL_SPI_Transmit(&hspi6,&cmd,1,1000);
}

static void LCD_WriteData_8bit(uint8_t dat)
{
    LCD_WR_RS(1);
     HAL_SPI_Transmit(&hspi6,&dat,1,1000);
}


static void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* 指定X方向操作区域 */
    LCD_WriteCommand(0x2a);
    LCD_WriteData_8bit(x1 >> 8);
    LCD_WriteData_8bit(x1);
    LCD_WriteData_8bit(x2 >> 8);
    LCD_WriteData_8bit(x2);

    /* 指定Y方向操作区域 */
    LCD_WriteCommand(0x2b);
    LCD_WriteData_8bit(y1 >> 8);
    LCD_WriteData_8bit(y1);
    LCD_WriteData_8bit(y2 >> 8);
    LCD_WriteData_8bit(y2);

    /* 发送该命令，LCD开始等待接收显存数据 */
    LCD_WriteCommand(0x2C);
}
/**
 * @brief   以一种颜色清空LCD屏
 * @param   color —— 清屏颜色(16bit)
 * @return  none
 */
void LCD_Fill_first_half(uint8_t *data)
{
    /* 指定显存操作地址为全屏*/
    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height/2 - 1);
    LCD_WR_RS(1);/* 指定接下来的数据为数据 */
	/* 写前半屏*/

	HAL_SPI_Transmit(&hspi6,data, LCD_RAM_SIZE/2, HAL_MAX_DELAY);
}
void LCD_Fill_last_half(uint8_t *data)
{
    /* 指定显存操作地址为全屏*/
    LCD_Address_Set(0, LCD_Height/2, LCD_Width - 1, LCD_Height - 1);
    LCD_WR_RS(1);/* 指定接下来的数据为数据 */
    /*写后半屏*/
    HAL_SPI_Transmit(&hspi6,data+LCD_RAM_SIZE/2, LCD_RAM_SIZE/2, HAL_MAX_DELAY);
}

void LCD_Init(void)
{
 	LCD_WriteCommand(0x36);       // 显存访问控制 指令，用于设置访问显存的方式
	LCD_WriteData_8bit(0x00);     // 配置成 从上到下、从左到右，RGB像素格式

	LCD_WriteCommand(0x3A);			// 接口像素格式 指令，用于设置使用 12位、16位还是18位色
	LCD_WriteData_8bit(0x05);     // 此处配置成 16位 像素格式

// 接下来很多都是电压设置指令，直接使用厂家给设定值
 	LCD_WriteCommand(0xB2);			
	LCD_WriteData_8bit(0x0C);
	LCD_WriteData_8bit(0x0C); 
	LCD_WriteData_8bit(0x00); 
	LCD_WriteData_8bit(0x33); 
	LCD_WriteData_8bit(0x33); 			

	LCD_WriteCommand(0xB7);		   // 栅极电压设置指令	
	LCD_WriteData_8bit(0x35);     // VGH = 13.26V，VGL = -10.43V

	LCD_WriteCommand(0xBB);			// 公共电压设置指令
	LCD_WriteData_8bit(0x19);     // VCOM = 1.35V

	LCD_WriteCommand(0xC0);
	LCD_WriteData_8bit(0x2C);

	LCD_WriteCommand(0xC2);       // VDV 和 VRH 来源设置
	LCD_WriteData_8bit(0x01);     // VDV 和 VRH 由用户自由配置

	LCD_WriteCommand(0xC3);			// VRH电压 设置指令  
	LCD_WriteData_8bit(0x12);     // VRH电压 = 4.6+( vcom+vcom offset+vdv)
				
	LCD_WriteCommand(0xC4);		   // VDV电压 设置指令	
	LCD_WriteData_8bit(0x20);     // VDV电压 = 0v

	LCD_WriteCommand(0xC6); 		// 正常模式的帧率控制指令
	LCD_WriteData_8bit(0x0F);   	// 设置屏幕控制器的刷新帧率为60帧    

	LCD_WriteCommand(0xD0);			// 电源控制指令
	LCD_WriteData_8bit(0xA4);     // 无效数据，固定写入0xA4
	LCD_WriteData_8bit(0xA1);     // AVDD = 6.8V ，AVDD = -4.8V ，VDS = 2.3V

	LCD_WriteCommand(0xE0);       // 正极电压伽马值设定
	LCD_WriteData_8bit(0xD0);
	LCD_WriteData_8bit(0x04);
	LCD_WriteData_8bit(0x0D);
	LCD_WriteData_8bit(0x11);
	LCD_WriteData_8bit(0x13);
	LCD_WriteData_8bit(0x2B);
	LCD_WriteData_8bit(0x3F);
	LCD_WriteData_8bit(0x54);
	LCD_WriteData_8bit(0x4C);
	LCD_WriteData_8bit(0x18);
	LCD_WriteData_8bit(0x0D);
	LCD_WriteData_8bit(0x0B);
	LCD_WriteData_8bit(0x1F);
	LCD_WriteData_8bit(0x23);

	LCD_WriteCommand(0xE1);      // 负极电压伽马值设定
	LCD_WriteData_8bit(0xD0);
	LCD_WriteData_8bit(0x04);
	LCD_WriteData_8bit(0x0C);
	LCD_WriteData_8bit(0x11);
	LCD_WriteData_8bit(0x13);
	LCD_WriteData_8bit(0x2C);
	LCD_WriteData_8bit(0x3F);
	LCD_WriteData_8bit(0x44);
	LCD_WriteData_8bit(0x51);
	LCD_WriteData_8bit(0x2F);
	LCD_WriteData_8bit(0x1F);
	LCD_WriteData_8bit(0x1F);
	LCD_WriteData_8bit(0x20);
	LCD_WriteData_8bit(0x23);

	LCD_WriteCommand(0x21);       // 打开反显，因为面板是常黑型，操作需要反过来

 // 退出休眠指令，LCD控制器在刚上电、复位时，会自动进入休眠模式 ，因此操作屏幕之前，需要退出休眠  
	LCD_WriteCommand(0x11);       // 退出休眠 指令
   HAL_Delay(120);               // 需要等待120ms，让电源电压和时钟电路稳定下来

 // 打开显示指令，LCD控制器在刚上电、复位时，会自动关闭显示 
	LCD_WriteCommand(0x29);       // 打开显示   	

    /*打开显示*/
    LCD_PWR(1);
}


