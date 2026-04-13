#ifndef __IO_CONTROL_H
#define __IO_CONTROL_H

#include "config.h"

#define SW_1 BITBAND_PERI(P10OUT,0)
#define SW_2 BITBAND_PERI(P10OUT,1)
#define SW_3 BITBAND_PERI(P10OUT,2)
#define SW_4 BITBAND_PERI(P10OUT,3)

#define ADC_TO_MSP432 BITBAND_PERI(P10OUT,4)
#define ADC_TO_STM32 BITBAND_PERI(P10OUT,5)

#define PWM BITBAND_PERI(P9OUT,7)

void SW_Init(void);

void MSP432_ADC(void);
void STM32_ADC(void);

void AM_OUT(void);
void FM_OUT(void);
void CW_OUT(void);
void ASK_OUT(void);
void FSK_OUT(void);
void PSK_OUT(void);

#endif
