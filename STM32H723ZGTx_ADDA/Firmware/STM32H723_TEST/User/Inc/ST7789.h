#ifndef __ST7789_H
#define __ST7789_H
#include "main.h"
#include "spi.h"

#define	LCD_PWR(n)		(n?\
						HAL_GPIO_WritePin(LCD_Backlight_GPIO_Port,LCD_Backlight_Pin,GPIO_PIN_SET):\
						HAL_GPIO_WritePin(LCD_Backlight_GPIO_Port,LCD_Backlight_Pin,GPIO_PIN_RESET))
#define	LCD_WR_RS(n)	(n?\
						HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET):\
						HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET))



//LCD屏幕分辨率定义
#define LCD_Width   240
#define LCD_Height  240
#define LCD_RAM_SIZE LCD_Width*LCD_Height*2		//长240 宽240 色深2bit
#define Pixel_NUM (LCD_RAM_SIZE/2)

void LCD_Init(void);
void LCD_Fill_first_half(uint8_t *data);
void LCD_Fill_last_half(uint8_t *data);

#endif
