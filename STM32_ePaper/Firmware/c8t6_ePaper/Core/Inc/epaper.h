#ifndef __EPAPER_H
#define __EPAPER_H
#include "main.h"

// 212 x 104

#define MAX_LINE_BYTES 			13		// =104/8
#define MAX_COLUMN_BYTES  		212

#define ALLSCREEN_GRAGHBYTES	2756

///GPIO Settings//

#define EPD_W21_MOSI_0	HAL_GPIO_WritePin(GPIOA, MOSI_Pin, GPIO_PIN_RESET)
#define EPD_W21_MOSI_1	HAL_GPIO_WritePin(GPIOA, MOSI_Pin, GPIO_PIN_SET)

#define EPD_W21_CLK_0	HAL_GPIO_WritePin(GPIOA, SCK_Pin, GPIO_PIN_RESET)
#define EPD_W21_CLK_1	HAL_GPIO_WritePin(GPIOA, SCK_Pin, GPIO_PIN_SET)

#define EPD_W21_CS_0	HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_RESET)
#define EPD_W21_CS_1	HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_SET)

#define EPD_W21_DC_0	HAL_GPIO_WritePin(GPIOA, DC_Pin, GPIO_PIN_RESET)
#define EPD_W21_DC_1	HAL_GPIO_WritePin(GPIOA, DC_Pin, GPIO_PIN_SET)

#define EPD_W21_RST_0	HAL_GPIO_WritePin(GPIOA, RST_Pin, GPIO_PIN_RESET)
#define EPD_W21_RST_1	HAL_GPIO_WritePin(GPIOA, RST_Pin, GPIO_PIN_SET)

#define EPD_W21_BUSY_LEVEL 0
#define isEPD_W21_BUSY	HAL_GPIO_ReadPin(GPIOA, BUSY_Pin)

void EpaperIO_Init(void);
void Epaper_READBUSY(void);
void Epaper_Spi_WriteByte(uint8_t TxData);
void Epaper_Write_Command(uint8_t cmd);
void Epaper_Write_Data(uint8_t data);

void EPD_HW_Init(void); //Electronic paper initialization
void EPD_Update(void);
void EPD_DeepSleep(void);	

void EPD_WhiteScreen_Red(void);
void EPD_WhiteScreen_Black(void);
void EPD_WhiteScreen_White(void);

void EPD_ALL_image(const uint8_t *datas1, const uint8_t *datas2);

#endif
