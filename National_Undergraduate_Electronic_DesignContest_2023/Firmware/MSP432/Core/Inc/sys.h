#ifndef __SYS_H
#define __SYS_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define TICK_FREQ_10HZ         100U
#define TICK_FREQ_100HZ        10U
#define TICK_FREQ_1KHZ         1U
#define TICK_FREQ_DEFAULT      TICK_FREQ_1KHZ

extern __IO uint32_t uwTick;

void SystemClock_Config(void);

#endif
