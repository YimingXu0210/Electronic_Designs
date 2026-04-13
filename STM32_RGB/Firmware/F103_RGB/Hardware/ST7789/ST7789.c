#include "st7789.h"
#include "string.h"
#include "stdlib.h"

static SPI_HandleTypeDef *hspi_lcd;

// 内联函数用于控制引脚
static inline void WR_LOW() { HAL_GPIO_WritePin(LCD_WR_PORT, LCD_WR_PIN, GPIO_PIN_RESET); }
static inline void WR_HIGH() { HAL_GPIO_WritePin(LCD_WR_PORT, LCD_WR_PIN, GPIO_PIN_SET); }
static inline void RES_LOW() { HAL_GPIO_WritePin(LCD_RES_PORT, LCD_RES_PIN, GPIO_PIN_RESET); }
static inline void RES_HIGH() { HAL_GPIO_WritePin(LCD_RES_PORT, LCD_RES_PIN, GPIO_PIN_SET); }
static inline void BLK_ON() { HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_SET); }
static inline void BLK_OFF() { HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_RESET); }

// 发送命令（硬件CS自动控制）
void ST7789_WriteCommand(uint8_t cmd) {
    WR_LOW();  // WR下降沿开始写入
    HAL_SPI_Transmit(hspi_lcd, &cmd, 1, HAL_MAX_DELAY);
    WR_HIGH(); // WR上升沿结束写入
    HAL_Delay(1); // 命令后延迟
}

// 发送数据（硬件CS自动控制）
void ST7789_WriteData(uint8_t data) {
    WR_LOW();
    HAL_SPI_Transmit(hspi_lcd, &data, 1, HAL_MAX_DELAY);
    WR_HIGH();
    HAL_Delay(1); // 数据后延迟
}

// 发送16位数据
void ST7789_WriteData16(uint16_t data) {
    uint8_t buffer[2] = {data >> 8, data & 0xFF};
    WR_LOW();
    HAL_SPI_Transmit(hspi_lcd, buffer, 2, HAL_MAX_DELAY);
    WR_HIGH();
}

// 发送数据缓冲区
void ST7789_WriteDataBuffer(uint16_t *data, uint32_t size) {
    WR_LOW();
    HAL_SPI_Transmit(hspi_lcd, (uint8_t*)data, size * 2, HAL_MAX_DELAY);
    WR_HIGH();
}

// 设置显示窗口
void ST7789_SetAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    ST7789_WriteCommand(0x2A); // 列地址设置
    ST7789_WriteData16(x1);
    ST7789_WriteData16(x2);
    
    ST7789_WriteCommand(0x2B); // 行地址设置
    ST7789_WriteData16(y1);
    ST7789_WriteData16(y2);
    
    ST7789_WriteCommand(0x2C); // 内存写入
}

// 初始化LCD
void ST7789_Init(SPI_HandleTypeDef *hspi) {
    hspi_lcd = hspi;
    
    // 初始化WR引脚
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LCD_WR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_WR_PORT, &GPIO_InitStruct);
    WR_HIGH(); // 初始化为高电平
    
    // 硬件复位
    RES_HIGH();
    HAL_Delay(5);
    RES_LOW();
    HAL_Delay(100);
    RES_HIGH();
    HAL_Delay(100);
    
    // 开启背光
    BLK_ON();
    HAL_Delay(100);
    
    //************* Start Initial Sequence ​**********//
    ST7789_WriteCommand(0x11); //Sleep out 
    HAL_Delay(120);              //Delay 120ms 
    //************* Start Initial Sequence ​**********// 
    ST7789_WriteCommand(0x36);
    if(USE_HORIZONTAL==0)ST7789_WriteData(0x00);
    else if(USE_HORIZONTAL==1)ST7789_WriteData(0xC0);
    else if(USE_HORIZONTAL==2)ST7789_WriteData(0x70);
    else ST7789_WriteData(0xA0);

    ST7789_WriteCommand(0x3A);     
    ST7789_WriteData(0x05);   

    ST7789_WriteCommand(0xB2);     
    ST7789_WriteData(0x1F);   
    ST7789_WriteData(0x1F);   
    ST7789_WriteData(0x00);   
    ST7789_WriteData(0x33);   
    ST7789_WriteData(0x33);   

    ST7789_WriteCommand(0xB7);     
    ST7789_WriteData(0x35);   

    ST7789_WriteCommand(0xBB);     
    ST7789_WriteData(0x2B);   //2b

    ST7789_WriteCommand(0xC0);     
    ST7789_WriteData(0x2C);   

    ST7789_WriteCommand(0xC2);     
    ST7789_WriteData(0x01);   

    ST7789_WriteCommand(0xC3);     
    ST7789_WriteData(0x0F);   

    ST7789_WriteCommand(0xC4);     
    ST7789_WriteData(0x20);   //VDV, 0x20:0v

    ST7789_WriteCommand(0xC6);     
    ST7789_WriteData(0x13);   //0x13:60Hz   

    ST7789_WriteCommand(0xD0);     
    ST7789_WriteData(0xA4);   
    ST7789_WriteData(0xA1);   

    ST7789_WriteCommand(0xD6);     
    ST7789_WriteData(0xA1);   //sleep in后，gate输出为GND

    ST7789_WriteCommand(0xE0);     
    ST7789_WriteData(0xF0);   
    ST7789_WriteData(0x04);   
    ST7789_WriteData(0x07);   
    ST7789_WriteData(0x04);   
    ST7789_WriteData(0x04);   
    ST7789_WriteData(0x04);   
    ST7789_WriteData(0x25);   
    ST7789_WriteData(0x33);   
    ST7789_WriteData(0x3C);   
    ST7789_WriteData(0x36);   
    ST7789_WriteData(0x14);   
    ST7789_WriteData(0x12);   
    ST7789_WriteData(0x29);   
    ST7789_WriteData(0x30);   

    ST7789_WriteCommand(0xE1);     
    ST7789_WriteData(0xF0);   
    ST7789_WriteData(0x02);   
    ST7789_WriteData(0x04);   
    ST7789_WriteData(0x05);   
    ST7789_WriteData(0x05);   
    ST7789_WriteData(0x21);   
    ST7789_WriteData(0x25);   
    ST7789_WriteData(0x32);   
    ST7789_WriteData(0x3B);   
    ST7789_WriteData(0x38);   
    ST7789_WriteData(0x12);   
    ST7789_WriteData(0x14);   
    ST7789_WriteData(0x27);   
    ST7789_WriteData(0x31);   

    ST7789_WriteCommand(0xE4);     
    ST7789_WriteData(0x1D);   //使用240根gate  (N+1)*8
    ST7789_WriteData(0x00);   //设定gate起点位置
    ST7789_WriteData(0x00);   //当gate没有用完时，bit4(TMG)设为0

    ST7789_WriteCommand(0x21);     

    ST7789_WriteCommand(0x29);     
}

// 填充屏幕
void ST7789_FillScreen(uint16_t color) {
    uint16_t buffer[ST7789_WIDTH];
    
    for(int i = 0; i < ST7789_WIDTH; i++) {
        buffer[i] = color;
    }
    
    ST7789_SetAddressWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);
    
    for(int y = 0; y < ST7789_HEIGHT; y++) {
        ST7789_WriteDataBuffer(buffer, ST7789_WIDTH);
    }
}

// 绘制像素点
void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if(x >= ST7789_WIDTH || y >= ST7789_HEIGHT) return;
    
    ST7789_SetAddressWindow(x, y, x, y);
    ST7789_WriteData16(color);
}

// 绘制图像
void ST7789_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data) {
    if(x + w > ST7789_WIDTH || y + h > ST7789_HEIGHT) return;
    
    ST7789_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    ST7789_WriteDataBuffer((uint16_t*)data, w * h);
}

// 设置显示方向
void ST7789_SetRotation(uint8_t rotation) {
    ST7789_WriteCommand(0x36); // 内存数据访问控制
    
    switch(rotation) {
        case 0: // 纵向
            ST7789_WriteData(0x00);
            break;
        case 1: // 横向
            ST7789_WriteData(0xC0);
            break;
        case 2: // 倒置纵向
            ST7789_WriteData(0x70);
            break;
        case 3: // 倒置横向
            ST7789_WriteData(0xA0);
            break;
        default:
            ST7789_WriteData(0x00);
    }
}

// 颜色反转
void ST7789_InvertColors(uint8_t invert) {
    ST7789_WriteCommand(invert ? 0x21 : 0x20);
}
