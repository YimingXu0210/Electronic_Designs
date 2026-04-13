/*
 * @Author: m-RNA m-RNA@qq.com
 * @Date: 2022-08-06 23:46:21
 * @LastEditors: m-RNA m-RNA@qq.com
 * @LastEditTime: 2022-08-07 00:35:59
 * @FilePath: \MDK-ARMe:\RNA_2021_Game_A\MCU Project\BLL\Inc\windowfunction.h
 * @Description: 加窗的相关文件
 */

#ifndef __BLL_WINDOWFUNCTION_H__
#define __BLL_WINDOWFUNCTION_H__
#include "config.h"

typedef enum
{
    WithoutWinFun, // 不加窗
    Boxcar,        // 矩形窗（不加窗）
    Triang,        // 三角窗
    Hanning,       // 汉明窗
    Hamming,       // 海明窗
    Blackman,      // 布莱克曼窗
    Flattop,       // 平顶窗
} Window_Function_Type;

/**
 * @description: 窗函数初始化
 * @param WinFun 窗函数类型
 *      @arg WithoutWinFun 不加窗
 *      @arg Boxcar   矩形窗（不加窗）
 *      @arg Triang   三角窗
 *      @arg Hanning  汉明窗
 *      @arg Hamming  海明窗
 *      @arg Blackman 布莱克曼窗
 *      @arg Flattop  平顶窗
 * @param Length 窗的长度
 */
void Window_Function_Init(Window_Function_Type WinFun, uint16_t Length);

/**
 * @description: 加窗函数
 * @param Data  加窗的原始数据
 * @param Index 数据索引
 * @return float 数据乘以窗数组对应索引的值
 */
float Window_Function_Add(int16_t Data, uint16_t Index);

#endif
