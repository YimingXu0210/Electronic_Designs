#ifndef __DELAY_H
#define __DELAY_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void Delay_Init(void);

void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);

#endif
