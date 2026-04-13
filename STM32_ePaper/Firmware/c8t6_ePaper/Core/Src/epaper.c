#include "epaper.h"

void Epaper_Spi_WriteByte(uint8_t TxData) {			
	uint8_t TempData;
	TempData = TxData;

	EPD_W21_CLK_0;  
	for(uint8_t scnt = 0; scnt < 8; scnt++) { 
		if(TempData & 0x80)
			EPD_W21_MOSI_1 ;
		else
			EPD_W21_MOSI_0 ;
		
		EPD_W21_CLK_1;  
		EPD_W21_CLK_0;  
		TempData = TempData << 1;
	}
	
}

void Epaper_READBUSY(void) { 
	while(1) {	  //=1 BUSY
		if(isEPD_W21_BUSY == EPD_W21_BUSY_LEVEL) break;;
	}  
}

void Epaper_Write_Command(uint8_t cmd) {
	EPD_W21_CS_1;
	EPD_W21_CS_0;
	EPD_W21_DC_0;  // D/C#   0:command  1:data

	Epaper_Spi_WriteByte(cmd);
	EPD_W21_CS_1;
}

void Epaper_Write_Data(uint8_t data) {
	EPD_W21_CS_1;
	EPD_W21_CS_0;
	EPD_W21_DC_1;  // D/C#   0:command  1:data

	Epaper_Spi_WriteByte(data);
	EPD_W21_CS_1;
}

void EPD_HW_Init(void) {	
	EPD_W21_RST_0;     
	HAL_Delay(1);
	EPD_W21_RST_1; //hard reset  
	HAL_Delay(1);

	Epaper_READBUSY();
	Epaper_Write_Command(0x12); // soft reset
	Epaper_READBUSY();

	Epaper_Write_Command(0x74); //set analog block control       
	Epaper_Write_Data(0x54);
	Epaper_Write_Command(0x7E); //set digital block control          
	Epaper_Write_Data(0x3B);

	Epaper_Write_Command(0x01); //Driver output control      
	Epaper_Write_Data(0xF9);
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);

	Epaper_Write_Command(0x11); //data entry mode       
	Epaper_Write_Data(0x01);

	Epaper_Write_Command(0x44); //set Ram-X address start/end position   
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x0C);    //0x0C-->(12+1)*8=112

	Epaper_Write_Command(0x45); //set Ram-Y address start/end position          
	Epaper_Write_Data(0xD3);    //0xD3-->(211+1)=212
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00); 

	Epaper_Write_Command(0x3C); //BorderWavefrom
	Epaper_Write_Data(0x01);
	
	Epaper_Write_Command(0x18); 
	Epaper_Write_Data(0x80);	
	
	Epaper_Write_Command(0x4E); // set RAM x address count to 0;
	Epaper_Write_Data(0x00);
	Epaper_Write_Command(0x4F); // set RAM y address count to 0xD3-->(211+1)=212;    
	Epaper_Write_Data(0xD3);
	Epaper_Write_Data(0x00);
	Epaper_READBUSY();
}

void EPD_Update(void) {   
	Epaper_Write_Command(0x22);
	Epaper_Write_Data(0xF7);
	Epaper_Write_Command(0x20);
	Epaper_READBUSY();
}

void EPD_DeepSleep(void) {  	
	Epaper_Write_Command(0x10); //enter deep sleep
	Epaper_Write_Data(0x01); 
}

void EPD_WhiteScreen_Red(void) {
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(uint8_t k = 0; k < MAX_COLUMN_BYTES; k++) {
		for(uint8_t i = 0; i < MAX_LINE_BYTES; i++) {
			Epaper_Write_Data(0xFF);
		}
	}
  
    Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
	for(uint8_t k = 0; k < MAX_COLUMN_BYTES; k++) {
		for(uint8_t i = 0; i < MAX_LINE_BYTES; i++) {
			Epaper_Write_Data(0xFF);
		}
	}
	
	EPD_Update();
}

void EPD_WhiteScreen_Black(void) {
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(uint8_t k = 0; k < MAX_COLUMN_BYTES; k++) {
		for(uint8_t i = 0; i < MAX_LINE_BYTES; i++) {
			Epaper_Write_Data(0x00);
		}
	}
  
    Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
	for(uint8_t k = 0; k < MAX_COLUMN_BYTES; k++) {
		for(uint8_t i = 0; i < MAX_LINE_BYTES; i++) {
			Epaper_Write_Data(0x00);
		}
	}
	
	EPD_Update();
}

void EPD_WhiteScreen_White(void) {
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(uint8_t k = 0; k < MAX_COLUMN_BYTES; k++) {
		for(uint8_t i = 0; i < MAX_LINE_BYTES; i++) {
			Epaper_Write_Data(0xFF);
		}
	}
  
    Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
	for(uint8_t k = 0; k < MAX_COLUMN_BYTES; k++) {
		for(uint8_t i = 0; i < MAX_LINE_BYTES; i++) {
			Epaper_Write_Data(0x00);
		}
	}
	
	EPD_Update();

}

void EPD_ALL_image(const uint8_t *datas1, const uint8_t *datas2) {
	Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(uint16_t i = 0; i < ALLSCREEN_GRAGHBYTES; i++) {               
		Epaper_Write_Data(*datas1); 
		datas1++;
	}	
	
	
	Epaper_Write_Command(0x26);   //write RAM for red(1)/white (0)
	for(uint16_t i = 0; i < ALLSCREEN_GRAGHBYTES; i++) {                   
//		Epaper_Write_Data(*datas2); 
//		datas2++;
		Epaper_Write_Data(0x00);
	}
	
	EPD_Update();

}
