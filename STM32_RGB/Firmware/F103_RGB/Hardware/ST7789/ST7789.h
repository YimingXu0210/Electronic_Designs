#ifndef ST7789_H
#define ST7789_H

#include "stm32f1xx_hal.h"

// LCD尺寸定义
#define ST7789_WIDTH  240
#define ST7789_HEIGHT 240

// 屏幕方向设置
#define USE_HORIZONTAL 0  // 0-竖屏 1-横屏 2-竖屏倒置 3-横屏倒置

// 颜色定义 (RGB565)
#define WHITE     0xFFFF
#define BLACK     0x0000
#define RED       0xF800
#define GREEN     0x07E0
#define BLUE      0x001F
#define YELLOW    0xFFE0
#define CYAN      0x07FF
#define MAGENTA   0xF81F

// 控制引脚定义（根据实际连接修改）
#define LCD_WR_PIN    GPIO_PIN_6  // 写使能信号
#define LCD_WR_PORT   GPIOA
#define LCD_RES_PIN   GPIO_PIN_2  // 复位
#define LCD_RES_PORT  GPIOA
#define LCD_BLK_PIN   GPIO_PIN_3  // 背光
#define LCD_BLK_PORT  GPIOA

// 函数声明
void ST7789_Init(SPI_HandleTypeDef *hspi);
void ST7789_WriteCommand(uint8_t cmd);
void ST7789_WriteData(uint8_t data);
void ST7789_WriteData16(uint16_t data);
void ST7789_WriteDataBuffer(uint16_t *data, uint32_t size);
void ST7789_SetAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ST7789_FillScreen(uint16_t color);
void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7789_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
void ST7789_SetRotation(uint8_t rotation);
void ST7789_InvertColors(uint8_t invert);

#endif // ST7789_H