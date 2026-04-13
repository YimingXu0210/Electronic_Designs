#ifndef __UART_HMI_H__
#define __UART_HMI_H__

#include "config.h"

extern urx_buffer HMI_RX;

void HMI_RX_proc(void);

void Main_dispaly(void);
void Page_switching(uint8_t Modulation_signal_type);
void AM_signal_display(uint32_t Sinusoidal_signal_modulation_frequency, float Amplitude_modulation_factor);
void FM_signal_display(uint32_t Maximum_frequency_offset, float Frequency_modulation_coefficient, uint32_t Sinusoidal_signal_modulation_frequency);
void CW_signal_display(void);
void ASK_signal_display(uint32_t Binary_code_rate);
void FSK_signal_display(uint32_t Binary_code_rate, float Frequency_shift_keying_factor);
void PSK_signal_display(uint32_t Binary_code_rate);

#endif
