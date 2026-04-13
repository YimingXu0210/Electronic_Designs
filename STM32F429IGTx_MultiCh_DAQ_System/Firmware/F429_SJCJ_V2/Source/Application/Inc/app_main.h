#ifndef __APP_MAIN_H
#define __APP_MAIN_H

#include "main.h"

#include "delay.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define M_PI       3.14159265358979323846

#define TMP_CH		28
#define TMP_GP		7
#define PRS_CH		14
#define FRT_CH		2

#define PRS_GET_NUM	5


typedef struct {
	float Temperature[TMP_CH];
	float Pressure[PRS_CH];
	float Flow_Rate[FRT_CH];
} Data_Docker_Struct;

int app_main(void);

#endif /* __APP_MAIN_H */
