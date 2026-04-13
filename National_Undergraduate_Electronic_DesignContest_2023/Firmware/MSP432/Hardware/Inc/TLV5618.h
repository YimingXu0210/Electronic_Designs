#ifndef __TLV5618_H
#define __TLV5618_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define  CS_H()  GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN5)
#define  CS_L()  GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN5)

void TCL5618_init(void);
void write_data_to_TCL5618(uint16_t data);

void DAC5618_translate_a(uint16_t n);
void DAC5618_translate_b(uint16_t n);

#endif