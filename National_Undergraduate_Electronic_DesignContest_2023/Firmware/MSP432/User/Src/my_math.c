#include "my_math.h"

#if (FFT_CALCULATE_NUM == 16u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len16
#elif (FFT_CALCULATE_NUM == 32u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len32
#elif (FFT_CALCULATE_NUM == 64u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len64
#elif (FFT_CALCULATE_NUM == 128u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len128
#elif (FFT_CALCULATE_NUM == 256u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len256
#elif (FFT_CALCULATE_NUM == 512u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len512
#elif (FFT_CALCULATE_NUM == 1024u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len1024
#elif (FFT_CALCULATE_NUM == 2048u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len2048
#elif (FFT_CALCULATE_NUM == 4096u)
#define ARM_FFT_USING_STRUCTURE arm_cfft_sR_f32_len4096
#endif

#define FFT_To_Am_IndexErrorRange 4
#define Bezier_num                9
//static float Synthetic_WaveBuf[Signal_Synthesizer_Wave_Length_MAX];
static float FFT_Input_Buf[FFT_CALCULATE_NUM * 2];
SpectrumInfo spectrumInfo;

uint8_t AFP_SK;


//float Bezier_Curve[Bezier_num] = {
//	0.765, 0.720, 0.671, 0.620, 0.567, 0.512, 0.455, 0.340, 0.282,
//	0.224, 0.167, 0.110, 0.056, 0.003, 0.048, 0.097, 0.142, 0.185, 0.224,
//	0.260, 0.292, 0.320, 0.344, 0.364, 0.380, 0.392,  
//	0.397, 0.389, 0.377, 0.361, 0.342, 0.321, 0.296, 0.269, 0.240, 0.210,
//	0.178};

//float dot_Bezier_Curve[Bezier_num] = {
//	1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.8, 1.9,
//	2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9,
//	3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6,
//	4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9,
//	5.0};

    
float Bezier_Curve[Bezier_num] = {
	0.765, 0.512, 
	0.224, 0.048,
	0.260, 0.380,
	0.397, 0.321,
	0.178};

float dot_Bezier_Curve[Bezier_num] = {
	1.0, 1.5,
	2.0, 2.5,
	3.0, 3.5,
	4.0, 4.5,
	5.0};
    
uint16_t Bezier_diff[Bezier_num]={0};
uint16_t min_diff=10;
float sum_Mf=0,mean_Mf=0;    
uint8_t min_diff_index;

void extractSpectrumInfo(float spectrum[], int spectrumSize, SpectrumInfo* spectrumInfo, float threshold) {
    int numPeaks = 0;
    for (int i = 5; i < spectrumSize - 1; i++) {
        if (spectrum[i] > spectrum[i - 1] && spectrum[i] > spectrum[i - 2] && spectrum[i] > spectrum[i - 3] && spectrum[i] > spectrum[i - 4]
            && spectrum[i] > spectrum[i + 1] && spectrum[i] > spectrum[i + 2] && spectrum[i] > spectrum[i + 3] && spectrum[i] > spectrum[i + 4]
            && spectrum[i] > threshold) 
            {
            spectrumInfo->peaks[numPeaks].index = i;
            spectrumInfo->peaks[numPeaks].value = spectrum[i];
            numPeaks++;
            if (numPeaks >= MAX_PEAKS) {
                break;
            }
        }
    }
    spectrumInfo->numPeaks = numPeaks;
}

void Identify_AFP_SK(float *Am_Pointer) {
	uint32_t Rc_True;
	static uint32_t Rc;
	
	uint32_t Get_High_Cap, Get_Total_Cap, EX_Freq;
	float Get_Freq, Get_Duty;
	
	AFP_SK = 0;
	
	printf_uart1("\x01");
	LED_RED_On();
	uint8_t ccnnt = 0;
	while(!STM_STAT) {
		Rx_A1_proc();
		delay_ms(10);
		ccnnt++;
		if(ccnnt >= 100) {
			printf_uart1("\x01");
			ccnnt = 0;
		}
	}
	STM_STAT = 0;
	LED_RED_Off();
//	
//	printf("{afp}%d\r\n", AFP_SK);
	
	if(AFP_SK == 1) {
		
		ASK_OUT();
		
		Cap_Get_Start(&Get_High_Cap, &Get_Total_Cap, &Get_Freq, &Get_Duty);
		Rc_True = Get_Freq * 2;
		
		if(Rc_True > 5500 && Rc_True < 6500) Rc = 6;
		else if(Rc_True > 7500 && Rc_True < 8500) Rc = 8;
		else if(Rc_True > 9500 && Rc_True < 10500) Rc = 10;
		
		ASK_signal_display(Rc);
		
	} else if(AFP_SK == 2) {
		float H;
		
		FSK_OUT();
		
		Cap_Get_Start(&Get_High_Cap, &Get_Total_Cap, &Get_Freq, &Get_Duty);
		Rc_True = Get_Freq * 2;
		
		if(Rc_True > 5500 && Rc_True < 6500) Rc = 6;
		else if(Rc_True > 7500 && Rc_True < 8500) Rc = 8;
		else if(Rc_True > 9500 && Rc_True < 10500) Rc = 10;
		
		Int_Get_Start(&EX_Freq);
//		printf("{EX}%d\r\n", EX_Freq);
		
		int32_t fc1 = 2000000 - (2000000 - EX_Freq) * 2;
		
//		printf("{EX2}%d\r\n", fc1);
		
		H = abs(2000000 - fc1) * 1.f / Rc / 1000.f;
		
		FSK_signal_display(Rc, H);
		
	} else if(AFP_SK == 3) {
		
		MSP432_ADC();
		
		Bp_Fs_Change_F(1000000);
		Adc_Start();
		CalculateAmplitude_By_FFT(Am_Pointer, Signal_ADC_Data, 1);				// 去除平均量
		Rc_True = (Max_Float_WithinRange(Am_Pointer, FFT_CALCULATE_NUM/ADC_SAMPLING_NUM, FFT_CALCULATE_NUM/2 - 1) * 1.f / FFT_CALCULATE_NUM) * 1000000 * 2;
		
		if(Rc_True > 5500 && Rc_True < 6500) {Rc = 6;printf_uart1("\x06");}
		else if(Rc_True > 7500 && Rc_True < 8500) {Rc = 8;printf_uart1("\x08");}
		else if(Rc_True > 9500 && Rc_True < 10500) {Rc = 10;printf_uart1("\x0A");}
		
		PSK_signal_display(Rc);
		
		PSK_OUT();
		
		if(Rc == 6) {
			while(1) {
				PWM ^= 1;
				delay_us(164);
			}
		} else if(Rc == 8) {
			while(1) {
				PWM ^= 1;
				delay_us(124);
			}
		} else if(Rc == 10) {
			while(1) {
				PWM ^= 1;
				delay_us(99);
			}
		}
		
//		for(int i = 0; i < ADC_SAMPLING_NUM; i++) {
//			printf("{Signal_ADC_Data}%d\r\n", Signal_ADC_Data[i]);
//		}

//		for(int i = 0; i < FFT_CALCULATE_NUM / 2; i++) {
//			printf("{Amplitude_Data}%.4f\r\n", Am_Pointer[i]);
//		}
	}
	
//	printf("{Rc_Ture}%d\r\n", Rc_True);
	
}

void Identify_AFC(float *Am_Pointer, uint8_t *AFC, float *M, uint32_t *Freq, uint32_t *Freq_Dev) {
	uint32_t Df;
	uint32_t FS;
	
	Df = 10000;
	FS = Bp_Fs_Adjust(2000000 - Df, 2000000 + Df);
//	printf("{FS}%d\r\n", FS);
	
	Bp_Fs_Change_F(FS);
	Adc_Start();
	CalculateAmplitude_By_FFT(Am_Pointer, Signal_ADC_Data, 1);				// 去除平均量
//	for(int i = 0; i < FFT_CALCULATE_NUM / 2; i++) {
//		printf("{Amplitude_Data}%.4f\r\n", Am_Pointer[i]);
//	}
	extractSpectrumInfo(Am_Pointer, FFT_CALCULATE_NUM / 2, &spectrumInfo, 15000);
	
	*M = 0;
	*Freq = 0;
	*Freq_Dev = 0;
	
	if(spectrumInfo.numPeaks == 1) {			// CW
		
		*AFC = 1;
		
		CW_signal_display();
		CW_OUT();
		
	} else if(spectrumInfo.numPeaks == 3) {		// AM
		*AFC = 2;
		
		uint8_t AM_Max_indx;
		
		if(spectrumInfo.peaks[0].value > spectrumInfo.peaks[1].value) {
			if(spectrumInfo.peaks[0].value > spectrumInfo.peaks[2].value) {
				AM_Max_indx = 0;
				
				*M = (spectrumInfo.peaks[1].value + spectrumInfo.peaks[2].value) / spectrumInfo.peaks[0].value;
				
			} else {
				AM_Max_indx = 2;
				
				*M = (spectrumInfo.peaks[0].value + spectrumInfo.peaks[1].value) / spectrumInfo.peaks[2].value;
				
			}
		} else {
			if(spectrumInfo.peaks[1].value > spectrumInfo.peaks[2].value) {
				AM_Max_indx = 1;
				
				*M = (spectrumInfo.peaks[0].value + spectrumInfo.peaks[2].value) / spectrumInfo.peaks[1].value;
				
			} else {
				AM_Max_indx = 2;
				
				*M = (spectrumInfo.peaks[0].value + spectrumInfo.peaks[1].value) / spectrumInfo.peaks[2].value;
				
			}
		}
		
		*Freq = (spectrumInfo.peaks[2].index - spectrumInfo.peaks[AM_Max_indx].index) * 1.f / FFT_CALCULATE_NUM * FS;
		
		AM_signal_display(*Freq, *M);
		AM_OUT();
	
	} else if(spectrumInfo.numPeaks > 3) {		// FM
		
		*AFC = 3;
		
//		Df = 30000;
//		FS = Bp_Fs_Adjust(2000000 - Df, 2000000 + Df);
		FS = 150000;
		
		Bp_Fs_Change_F(FS);
		Adc_Start();
		CalculateAmplitude_By_FFT(Am_Pointer, Signal_ADC_Data, 0);

		extractSpectrumInfo(Am_Pointer, FFT_CALCULATE_NUM, &spectrumInfo, 15000);
		
		*Freq = abs(spectrumInfo.peaks[spectrumInfo.numPeaks / 4].index - spectrumInfo.peaks[spectrumInfo.numPeaks / 4 + 1].index) * 1.f / FFT_CALCULATE_NUM * FS;
		
//		Max_Float_WithinRange(Am_Pointer, 1370 - 10, 1370 + 10);
		
//		printf("{2M}%d\r\n", Max_Float_WithinRange(Am_Pointer, 1370 - 10, 1370 + 10));
//		printf("{2M}%.4f\r\n", Am_Pointer[Max_Float_WithinRange(Am_Pointer, 1370 - 10, 1370 + 10)]);
//		printf("{2M}%.4f\r\n", Am_Pointer[Max_Float_WithinRange(Am_Pointer, 1370 - 10, 1370 + 10)] / Am_Pointer[0]);
		
		for(int j = 0; j < 20; j++)
        {
            printf("{2M2}%.4f\r\n", Am_Pointer[Max_Float_WithinRange(Am_Pointer, 1370 - 10, 1370 + 10)] / Am_Pointer[0] * 3.29f);
            sum_Mf += Am_Pointer[Max_Float_WithinRange(Am_Pointer, 1370 - 10, 1370 + 10)] / Am_Pointer[0] * 3.29f;
            Adc_Start();
		    CalculateAmplitude_By_FFT(Am_Pointer, Signal_ADC_Data, 0);
        }
        
        mean_Mf = sum_Mf/20+0.015;
        printf("{2M2}%.4f\r\n", mean_Mf);
        
        
        for(int i = 0; i < Bezier_num; i++)
        {
            Bezier_diff [i] = abs((int)((mean_Mf - Bezier_Curve[i])*10000));
            if(Bezier_diff [i] < min_diff)
            {
                min_diff = Bezier_diff [i];
                min_diff_index = i;
            }
        }
        printf("{2M1}%.4f\r\n",dot_Bezier_Curve[min_diff_index]);
        *M = dot_Bezier_Curve[min_diff_index];
        
        printf("{2M1}%.4f\r\n",* M);
        printf("{2M1}%d\r\n",*Freq_Dev);
        sum_Mf = 0;
        min_diff=1000;
		
		if(*Freq < 1000) *Freq += 12;
		else if(*Freq < 2000) *Freq += 23;
		else if(*Freq < 3010) *Freq -= 2;
		else if(*Freq < 4000) *Freq += 9;
		else if(*Freq < 4990) *Freq += 20;
		*Freq_Dev =(uint32_t) (*M * (*Freq));
		FM_signal_display(*Freq_Dev, *M, *Freq);
		FM_OUT();
		
	}
	
//	for (int i = 0; i < spectrumInfo.numPeaks; i++) {
//		printf("{Vp_indx}%d, %d, %f\n", i, spectrumInfo.peaks[i].index, spectrumInfo.peaks[i].value);
//	}

	for(int i = 0; i < ADC_SAMPLING_NUM; i++) {
		printf("{Signal_ADC_Data}%d\r\n", Signal_ADC_Data[i]);
	}


}

/*
 * 提示：同时找出最大值和最小值
 * 最优算法应为分治法 时间复杂度 O(3/2n)
 */

/* 找出最小值位置 */
uint16_t Min_Float(float Mag[], uint16_t len)
{
    uint16_t i, Fn_Num;
    Fn_Num = 0;
    Mag[Fn_Num] = Mag[0];
    for (i = 1; i < len; i++)
    {
        if (Mag[Fn_Num] > Mag[i])
        {
            Fn_Num = i;
        }
    }
    return Fn_Num;
}

/* 找出最大值位置 */
uint16_t Max_Unsigned(uint16_t Mag[], uint16_t len)
{
    uint16_t i, Fn_Num;
    Fn_Num = 0;
    Mag[Fn_Num] = Mag[0];
    for (i = 1; i < len; i++)
    {
        if (Mag[Fn_Num] < Mag[i])
        {
            Fn_Num = i;
        }
    }
    return Fn_Num;
}

/* 在一定范围内找出最大值位置 */
uint16_t Max_Float_WithinRange(float Data[], uint16_t Left, uint16_t Right)
{
    uint16_t i, MaxIndex;
    MaxIndex = Left;
    for (i = Left; i <= Right; ++i)
    {
        if (Data[MaxIndex] < Data[i])
        {
            MaxIndex = i;
        }
    }
    return MaxIndex;
}

/* 通过FFT 计算各个频率分量幅值 */
void CalculateAmplitude_By_FFT(float *Am_Pointer, uint16_t *SampleData_Pointer, uint8_t Remove_Avg_flag)
{
	q15_t ADC_Data_Avg = 0;
//	if(Remove_Avg_flag) arm_mean_q15((q15_t *)SampleData_Pointer, ADC_SAMPLING_NUM, &ADC_Data_Avg);			// 平均值直流量
	if(Remove_Avg_flag) ADC_Data_Avg = 8192;
	
	for(uint16_t i = ADC_SAMPLING_NUM * 2; i < FFT_CALCULATE_NUM * 2; i++) FFT_Input_Buf[i] = 0;
	
    for (uint16_t i = 0; i < ADC_SAMPLING_NUM; ++i)
    {
        FFT_Input_Buf[0 + (i << 1)] = Window_Function_Add(SampleData_Pointer[i] - ADC_Data_Avg, i); // 实部为数据	// 去直流
//		printf("{Signal_ADC_Data}%.4f\r\n", FFT_Input_Buf[0 + (i << 1)]);
        FFT_Input_Buf[1 + (i << 1)] = 0;                                             // 虚部为0
    }

    arm_cfft_f32(&ARM_FFT_USING_STRUCTURE, FFT_Input_Buf, 0, 1);        // FFT计算                                                 
    arm_cmplx_mag_f32(FFT_Input_Buf, Am_Pointer, FFT_CALCULATE_NUM); //把运算结果复数求模得幅值
}
