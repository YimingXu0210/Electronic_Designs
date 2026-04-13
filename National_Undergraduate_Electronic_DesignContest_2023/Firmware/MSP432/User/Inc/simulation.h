#ifndef __STIMULATION_H
#define __STIMULATION_H

#include "config.h"

#define SIMULATION_TIMES 7          // 仿真次数
#define SIMULATION_CCR_MAX 0xFFFF   // 定时器最大值 16位为 0xFFFF 32位为 0xFFFFFFFF
#define SIMULATION_CCR (SIMULATION_CCR_MAX & Simulation_CCR_Data[Simulation_Times_Index])

extern uint8_t Simulation_Times_Index;                 // 仿真次数索引
extern uint32_t Simulation_CCR_Data[SIMULATION_TIMES]; // 仿真捕获值

void Simulation_Set_Fs_ARR(uint16_t Fs_ARR);
void Simulate_Signal_Synthesizer(uint16_t *SimulateWaveData, uint16_t Length); // 信号合成
void Simulate_Signal_WaveformData(uint16_t *SimulateWaveData);            // 常见信号

#endif
