#include "windowfunction.h"

static float Win_Function_Buf[ADC_SAMPLING_NUM];

static void boxcar(uint16_t Length) // 矩形窗（不加窗）
{
    for (uint16_t i = 0; i < Length; i++)
    {
        Win_Function_Buf[i] = 1;
    }
}

static void triang(uint16_t Length) // 三角窗
{
    for (uint16_t i = 0; i < Length / 2; i++)
    {
        Win_Function_Buf[Length - i - 1] = Win_Function_Buf[i] = 2 * i / (float)Length;
    }
}

static void hanning(uint16_t Length) // 汉明窗
{
    for (uint16_t i = 0; i < Length; i++)
    {
        Win_Function_Buf[i] = 0.5f * (1 - arm_cos_f32(2 * PI * i / (Length + 1)));
    }
}

static void hamming(uint16_t Length) // 海明窗
{
    for (uint16_t i = 0; i < Length; i++)
    {
        Win_Function_Buf[i] = 0.54f - 0.46f * arm_cos_f32(2 * PI * i / (Length - 1));
    }
}

static void blackman(uint16_t Length) // 布莱克曼窗
{
    for (uint16_t i = 0; i < Length; i++)
    {
        Win_Function_Buf[i] = 0.42f - 0.5f * arm_cos_f32(2 * PI * i / (Length - 1)) + 0.08f * arm_cos_f32(4 * PI * i / (Length - 1));
    }
}

static void flattop(uint16_t Length) // 平顶窗
{
    for (uint16_t i = 0; i < Length; i++)
    {
        Win_Function_Buf[i] = (1 - 1.93f * arm_cos_f32(2 * PI * i / (Length - 1)) + 1.29f * arm_cos_f32(4 * PI * i / (Length - 1)) - 0.388f * arm_cos_f32(6 * PI * i / (Length - 1)) + 0.0322f * arm_cos_f32(8 * PI * i / (Length - 1))) / 4.634f;
    }
}

/* 窗函数初始化 */
void Window_Function_Init(Window_Function_Type WinFun, uint16_t Length)
{
    switch (WinFun)
    {
    case WithoutWinFun:
    case Boxcar:
        boxcar(Length); // 矩形窗（不加窗）
        break;
    case Triang:
        triang(Length); // 三角窗
        break;
    case Hanning:
        hanning(Length); // 汉明窗
        break;
    case Hamming:
        hamming(Length); // 海明窗
        break;
    case Blackman:
        blackman(Length); // 布莱克曼窗
        break;
    case Flattop:
        flattop(Length); // 平顶窗
        break;
    default:
        boxcar(Length); // 默认不加窗
        break;
    }
}

float Window_Function_Add(int16_t Data, uint16_t Index)
{
    return Data *= Win_Function_Buf[Index];
}
