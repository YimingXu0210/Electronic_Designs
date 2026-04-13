#ifndef __EEPROM_H
#define __EEPROM_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void EEPROM_Init(void);
void EE_Write(uint16_t add, const uint8_t *data, uint32_t len);
void EE_Read(uint16_t add, uint8_t *data, uint32_t len);
	
#endif
