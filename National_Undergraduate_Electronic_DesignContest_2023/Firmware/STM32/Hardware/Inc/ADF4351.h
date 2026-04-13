#ifndef _ADF4351_H_
#define _ADF4351_H_
#include "sys.h"

#if __MSP432P401R__

#define ADF4351_CE				BITBAND_PERI(P7OUT, 0)
#define ADF4351_LE				BITBAND_PERI(P7OUT, 1)
#define ADF4351_OUTPUT_DATA		BITBAND_PERI(P7OUT, 2)
#define ADF4351_CLK				BITBAND_PERI(P7OUT, 3)

#else

#define ADF4351_CE				PDout(7)
#define ADF4351_LE				PDout(2)
#define ADF4351_OUTPUT_DATA		PDout(4)
#define ADF4351_CLK				PDout(6)

#endif


#define ADF4351_RF_OFF		((u32)0XEC801C)

#define ADF4351_R0			((u32)0X2C8018)
#define ADF4351_R1			((u32)0X8029)
#define ADF4351_R2			((u32)0X10E42)
#define ADF4351_R3			((u32)0X4B3)
#define ADF4351_R4			((u32)0XEC803C)
#define ADF4351_R5			((u32)0X580005)

#define ADF4351_R1_Base		((u32)0X8001)
#define ADF4351_R4_Base		((u32)0X8C803C)
#define ADF4351_R4_ON		((u32)0X8C803C)
#define ADF4351_R4_OFF		((u32)0X8C883C)

#define ADF4351_PD_ON		((u32)0X10E42)
#define ADF4351_PD_OFF		((u32)0X10E02)


void ADF4351_IO_Init(void);
void ADF4351_Init(void);
void ReadToADF4351(u8 count, u8 *buf);
void WriteToADF4351(u8 count, u8 *buf);
void WriteOneRegToADF4351(u32 Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);		//	(xx.x) M Hz

#endif

