#include "UART_HMI.h"

urx_buffer HMI_RX;

static void Clear_Buffer(void) {
	memset(HMI_RX.Buffer, 0, HMI_RX.Size);
	HMI_RX.Flag = 0;
	HMI_RX.Size = 0;
}

void HMI_RX_proc(void) {
	if(uwTick - HMI_RX.uwTick_Rx < 10) return;
	HMI_RX.uwTick_Rx = uwTick;

	if(HMI_RX.Size) {
		
		HMI_printf("%s", HMI_RX.Buffer);
		
		Clear_Buffer();
	}
}
/******************************************************************************
* Name: The serial port screen shows the encapsulated function
* Author: Beginbegin
*******************************************************************************/
void Main_dispaly(void)
{
	HMI_printf("page MAIN\xff\xff\xff");
}


void Page_switching(uint8_t Modulation_signal_type)
{
	switch(Modulation_signal_type)
	{
		case 1:
			HMI_printf("page AM\xff\xff\xff");
			break;
		case 2:
			HMI_printf("page FM\xff\xff\xff");
			break;
		case 3:
			HMI_printf("page CW\xff\xff\xff");
			break;
		case 4:
			HMI_printf("page ASK\xff\xff\xff");
			break;
		case 5:
			HMI_printf("page FSK\xff\xff\xff");
			break;
		case 6:
			HMI_printf("page PSK\xff\xff\xff");
			break;
		default:
			break;
	}
}

void AM_signal_display(uint32_t Sinusoidal_signal_modulation_frequency, float Amplitude_modulation_factor)
{
	HMI_printf("page AM\xff\xff\xff");
	HMI_printf("t9.txt=\"%d Hz\"\xff\xff\xff", Sinusoidal_signal_modulation_frequency);
	HMI_printf("t10.txt=\"%f\"\xff\xff\xff", Amplitude_modulation_factor);
}

void FM_signal_display(uint32_t Maximum_frequency_offset, float Frequency_modulation_coefficient, uint32_t Sinusoidal_signal_modulation_frequency)
{
	HMI_printf("page FM\xff\xff\xff");
	HMI_printf("t9.txt=\"%d Hz\"\xff\xff\xff", Maximum_frequency_offset);
	HMI_printf("t10.txt=\"%f\"\xff\xff\xff", Frequency_modulation_coefficient);
    HMI_printf("t11.txt=\"%d Hz\"\xff\xff\xff", Sinusoidal_signal_modulation_frequency);
}

void CW_signal_display(void)
{
	HMI_printf("page CW\xff\xff\xff");
}

void ASK_signal_display(uint32_t Binary_code_rate)
{
	HMI_printf("page ASK\xff\xff\xff");
	HMI_printf("t9.txt=\"%d kbps\"\xff\xff\xff", Binary_code_rate);
}

void FSK_signal_display(uint32_t Binary_code_rate, float Frequency_shift_keying_factor)
{
	HMI_printf("page FSK\xff\xff\xff");
	HMI_printf("t9.txt=\"%d kbps\"\xff\xff\xff", Binary_code_rate);
	HMI_printf("t10.txt=\"%.2f\"\xff\xff\xff", Frequency_shift_keying_factor);
}

void PSK_signal_display(uint32_t Binary_code_rate)
{
	HMI_printf("page PSK\xff\xff\xff");
	HMI_printf("t9.txt=\"%d kbps\"\xff\xff\xff", Binary_code_rate);
}
