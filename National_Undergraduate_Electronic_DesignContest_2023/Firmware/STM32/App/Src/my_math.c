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

//static float Synthetic_WaveBuf[Signal_Synthesizer_Wave_Length_MAX];
static float FFT_Input_Buf[FFT_CALCULATE_NUM * 2];

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
uint16_t Max_Unsigned(float Mag[], uint16_t len)
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

///**
// * @brief  信号合成器
// * @param[out] Output    波形数据输出指针
// * @param[in]  Length    波形数据输出长度
// * @param[in]  F0_Vpp    基波幅值
// * @param[in]  NormAm    归一化幅值
// * @param[in]  Phase     相位
// * @param[in]  Precision 最高几次谐波分量
// */
//void Signal_Synthesizer(uint16_t *Output, uint16_t Length, uint16_t F0_Vpp, float *NormAm, float *Phase, uint8_t Precision)
//{
//    uint16_t i, j;
//    uint16_t MinIndex;

//    for (i = 0; i < Length; ++i)
//    {
//        Synthetic_WaveBuf[i] = arm_sin_f32(PI * i / ((float)(Length >> 1)) + Phase[0]);
//        for (j = 0; j < Precision - 1; ++j) // 各次谐波叠加
//        {
//            if (NormAm[j] == 0.0f)
//                continue;
//            Synthetic_WaveBuf[i] += arm_sin_f32(PI * i * (j + 2) / ((float)(Length >> 1)) + Phase[j]) * NormAm[j];
//        }
//    }

//    // 找出最小的小数的位置
//    MinIndex = Min_Float(Synthetic_WaveBuf, Length);
//    for (i = 0; i < Length; ++i)
//    {
//        // 将小数全转为以0为起点的正数 再乘以 F0_Vpp 变为整数
//        Output[i] = F0_Vpp * (Synthetic_WaveBuf[i] - Synthetic_WaveBuf[MinIndex]);
//    }
//}

///**
// * @brief  信号合成器
// * @param[out] Output    波形数据输出指针
// * @param[in]  Length    波形数据输出长度
// * @param[in]  Fx_Vpp    基波-谐波幅值
// * @param[in]  Phase     相位(弧度)
// * @param[in]  Precision 最高几次谐波分量
// */
//void Signal_Synthesizer_Vpp(uint16_t *Output, uint16_t Length, uint16_t *Fx_Vpp, float *Phase, uint8_t Precision)
//{
//    uint16_t i, j;
//    uint16_t MinIndex;

//    for (i = 0; i < Length; ++i)
//    {
//        Synthetic_WaveBuf[i] = (Fx_Vpp[0] / 2.0f) * arm_sin_f32(PI * i / ((float)(Length >> 1)) + Phase[0]);
//        for (j = 1; j < Precision; ++j) // 各次谐波叠加
//        {
//            if (Fx_Vpp[j] == 0)
//                continue;
//            Synthetic_WaveBuf[i] += (Fx_Vpp[j] / 2.0f) * arm_sin_f32(PI * i * (j + 1) / ((float)(Length >> 1)) + Phase[j]);
//        }
//    }
//    // 找出最小的小数的位置
//    MinIndex = Min_Float(Synthetic_WaveBuf, Length);
//    for (i = 0; i < Length; ++i)
//    {
//        // 将小数全转为以0为起点的正数 再乘以 F0_Vpp 变为整数
//        Output[i] = (Synthetic_WaveBuf[i] - Synthetic_WaveBuf[MinIndex]);
//    }
//}

/* 通过FFT 计算各个频率分量幅值 */
void CalculateAmplitude_By_FFT(float *Am_Pointer, uint16_t *SampleData_Pointer)
{
	q15_t ADC_Data_Avg;
	arm_mean_q15((q15_t *)SampleData_Pointer, ADC_SAMPLING_NUM, &ADC_Data_Avg);			// 平均值直流量
	
	for(uint16_t i = ADC_SAMPLING_NUM * 2; i < FFT_CALCULATE_NUM * 2; i++) FFT_Input_Buf[i] = 0;
	
    for (uint16_t i = 0; i < ADC_SAMPLING_NUM; ++i)
    {
        FFT_Input_Buf[0 + (i << 1)] = Window_Function_Add(SampleData_Pointer[i] - ADC_Data_Avg, i); // 实部为数据	// 去直流
		//printf("{Signal_ADC_Data}%.4f\r\n", FFT_Input_Buf[0 + (i << 1)]);
        FFT_Input_Buf[1 + (i << 1)] = 0;                                             // 虚部为0
    }

    arm_cfft_f32(&ARM_FFT_USING_STRUCTURE, FFT_Input_Buf, 0, 1);        // FFT计算
    arm_cmplx_mag_f32(FFT_Input_Buf, Am_Pointer, FFT_CALCULATE_NUM); //把运算结果复数求模得幅值
}

/*  计算各个谐波分量幅值相位 */
void NormalizedAm_And_CalculateTHD(float *Phase_Pointer, float *NormAm_Pointer, uint16_t *Fx_Vpp_Pointer, float *THD_Pointer, float *Am_Data_Pointer)
{
    uint16_t i;
    uint16_t Fx_Index[5] = {0};
    float Square_Sum = 0.0f;

    /* 找出基波位置 */
    Fx_Index[0] = Max_Float_WithinRange(Am_Data_Pointer, 1 + (FFT_To_Am_IndexErrorRange >> 1), (ADC_SAMPLING_NUM >> 1));
    Fx_Vpp_Pointer[0] = FX_VPP_MULTIPLE * Am_Data_Pointer[Fx_Index[0]] * ADC_RF_V_MV * 4 * FFT_CALCULATE_NUM / (ADC_SAMPLING_NUM * ADC_SAMPLING_NUM) / ADC_MAX;
    Phase_Pointer[0] = atan2f((FFT_Input_Buf[Fx_Index[0] << 1] + 1), (FFT_Input_Buf[Fx_Index[0] << 1] + 0));
    for (i = 1; i < 5; ++i)
    {
        /* 找出谐波位置 */
        Fx_Index[i] = Max_Float_WithinRange(
            Am_Data_Pointer,
            Fx_Index[0] * (i + 1) - (FFT_To_Am_IndexErrorRange >> 1),
            Fx_Index[0] * (i + 1) + (FFT_To_Am_IndexErrorRange >> 1)); // 在一定范围查找 较为准确

        /* 相位计算 */
        Phase_Pointer[i] = atan2f((FFT_Input_Buf[Fx_Index[i] << 1] + 1), (FFT_Input_Buf[Fx_Index[i] << 1] + 0));

        /* 幅值计算 */
        Fx_Vpp_Pointer[i] = FX_VPP_MULTIPLE * Am_Data_Pointer[Fx_Index[i]] * ADC_RF_V_MV * 4 * FFT_CALCULATE_NUM / (ADC_SAMPLING_NUM * ADC_SAMPLING_NUM) / ADC_MAX;

        /* 归一化幅值计算 */
        NormAm_Pointer[i - 1] = Am_Data_Pointer[Fx_Index[i]] / Am_Data_Pointer[Fx_Index[0]];

        /* THDx部分计算 */
        Square_Sum += Am_Data_Pointer[Fx_Index[i]] * Am_Data_Pointer[Fx_Index[i]]; // 平方和
    }
    arm_sqrt_f32(Square_Sum, THD_Pointer); // 开根号
    *THD_Pointer = *THD_Pointer * 100 / Am_Data_Pointer[Fx_Index[0]];

//    log_Fn_NAm_THD_data(Fx_Index, Phase_Pointer, Fx_Vpp_Pointer, NormAm_Pointer, *THD_Pointer);
}

///* 用归一化幅值+各分量相位 还原波形 */
//void Restore_Waveform(uint16_t *RestoreWaveform_Pointer, float *NormAm_Pointer, float *Phase_Pointer)
//{
//    log_detail("Transforming Normalized Am To Waveform Data...\r\n");

//    Signal_Synthesizer(RestoreWaveform_Pointer, OLED_X_MAX, 256, // 这个256是随便定的，目的是把小数转换为整数，使得波形细腻；OLED显示函数会进一步处理范围
//                       NormAm_Pointer, (void *)0, 5);

//    log_detail("Transforming Completed!\r\n");
//}

///* 用幅值+各分量相位 还原波形 */
//void Restore_Waveform_By_Vpp(uint16_t *RestoreWaveform, uint16_t *Fx_Vpp, float *Phase)
//{
//    LED_P_On();
//    log_detail("Transforming Normalized Am To Waveform Data...\r\n");

//    Signal_Synthesizer_Vpp(RestoreWaveform, OLED_X_MAX, Fx_Vpp, (void *)0, 5);

//    log_detail("Transforming Completed!\r\n");
//}
