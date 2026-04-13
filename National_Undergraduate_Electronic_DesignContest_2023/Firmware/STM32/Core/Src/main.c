/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "my_math.h"
#include "windowfunction.h"

#include "Key.h"
#include "UART_HMI.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define ASK 1
#define FSK 2
#define PSK 3

uint8_t mode=0,danfeng=0,mode_zu[100],count_ASK=0,count_PSK=0,count_FSK=0,count;
uint16_t num_zero=0;
int max_value_index=0,feng_value_index,feng_value_zu[400];
float feng_value[400],max_value=0;;
int sub_1_index,sub_2_index,sub_3_index,sub_4_index,sub_5_index;
float sub_1_value=0,sub_2_value=0,sub_3_value=0,sub_4_value=0,sub_5_value=0;

float Amplitude_Data[FFT_CALCULATE_NUM]; // 各个频率分量幅值(FFT后)	// 补0

float THDx = 0.0f;           // 失真度测量值
float NormalizedAm[4] = {0}; // 归一化幅值：2-5次谐波
float Phase[5] = {0};        // 各分量相位（占位，还没用上）
uint16_t Fx_Vpp[5] = {0};    // 幅值
uint32_t Freq_Base;
uint8_t feng_num=0;

uint8_t pwm_flag=0;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	delay_init(144);
	printf("HELLO\r\n");
//    HMI_printf("\xFF");
	__HAL_DMA_DISABLE_IT((&hadc1)->DMA_Handle, DMA_IT_HT);  //关掉DMA半中断
	
	HAL_UARTEx_ReceiveToIdle_IT(&huart2, HMI_RX.Buffer, BUFFER_SIZE);
    
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,199);
    __HAL_TIM_SET_AUTORELOAD(&htim3,199);
    
//    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,99);
//    __HAL_TIM_SET_AUTORELOAD(&htim3,199);
//    
//    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,124);
//    __HAL_TIM_SET_AUTORELOAD(&htim3,249);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {     
        HMI_RX_proc();
        if(pwm_flag)
        {   
            switch(pwm_freq)
            {
                case 1:
                    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,199);
                    __HAL_TIM_SET_AUTORELOAD(&htim3,199);
//                __HAL_TIM_SET_COUNTER(&htim3, 0xFFFF);
                
                break;
                case 2:
                    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,166);
                    __HAL_TIM_SET_AUTORELOAD(&htim3,332);
//                __HAL_TIM_SET_COUNTER(&htim3, 0xFFFF);
                break;
                case 3:
                    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,124);
                    __HAL_TIM_SET_AUTORELOAD(&htim3,249);
//                __HAL_TIM_SET_COUNTER(&htim3, 0xFFFF);
                break;
                case 4:
                    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,99);
                    __HAL_TIM_SET_AUTORELOAD(&htim3,199);
//                __HAL_TIM_SET_COUNTER(&htim3, 0xFFFF);
                break;
            }
            printf("{ARR}%d\r\n",TIM3->ARR);
            printf("{pwm_freq}%d\r\n",pwm_freq);
        }
        else
        {
            __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);
            __HAL_TIM_SET_AUTORELOAD(&htim3,199);
//            __HAL_TIM_SET_COUNTER(&htim3, 0xFFFF);
        }
        
        if(start_flag){
            HAL_GPIO_WritePin(GPIOB, LED0_Pin, 1);
            for(int k = 0;k < 100;k++){
                Adc_Fs_Change_F(1900000);
                Adc_Start();
                Window_Function_Init(Hanning, ADC_SAMPLING_NUM); // Boxcar, Triang, Hanning, Hamming, Blackman, Flattop
                CalculateAmplitude_By_FFT(Amplitude_Data, Signal_ADC_Data);
//        		for(int i = 0; i < FFT_CALCULATE_NUM/2; i++) {
//        			printf("{Amplitude_Data}%.1f\r\n", Amplitude_Data[i]);
//        		}
                
                for(int i = 3,j=0; i < FFT_CALCULATE_NUM/2-500; i++) {
                    feng_value_index = Max_Float_WithinRange(Amplitude_Data, i-3, i+3);
                    if(feng_value_index==i)
                    {
                        feng_value_zu[j]=feng_value_index;
                        feng_value[j++] = Amplitude_Data[feng_value_index];
//                        printf("{feng_value}%.1f\r\n", feng_value[j-1]);
//                        printf("{feng_value_zu}%d\r\n", feng_value_zu[j-1]);
                    }
                }
                max_value_index=Max_Unsigned(Amplitude_Data,FFT_CALCULATE_NUM/2);
                max_value=Amplitude_Data[max_value_index];
                
                for(int i = 0; i < 400; i++) 
                    if(feng_value[i]>sub_1_value)
                    {
                        if(feng_value_zu[i]!=max_value_index){
                            sub_1_value=feng_value[i];
                            sub_1_index=feng_value_zu[i];
                        }
                    }
                for(int i = 0; i < 400; i++)
                    if(feng_value[i]>sub_2_value)
                    {
                        if(feng_value_zu[i]!=max_value_index&&feng_value_zu[i]!=sub_1_index){
                        sub_2_value=feng_value[i];
                        sub_2_index=feng_value_zu[i];
                        }
                    }
                for(int i = 0; i < 400; i++)
                    if(feng_value[i]>sub_3_value)
                    {
                        if(feng_value_zu[i]!=max_value_index&&feng_value_zu[i]!=sub_1_index&&feng_value_zu[i]!=sub_2_index){
                        sub_3_value=feng_value[i];
                        sub_3_index=feng_value_zu[i];
                        }
                    }
                for(int i = 0; i < 400; i++)
                    if(feng_value[i]>sub_4_value)
                    {
                        if(feng_value_zu[i]!=max_value_index&&feng_value_zu[i]!=sub_1_index&&feng_value_zu[i]!=sub_2_index&&feng_value_zu[i]!=sub_3_index){
                        sub_4_value=feng_value[i];
                        sub_4_index=feng_value_zu[i];
                        }
                    }
                for(int i = 0; i < 400; i++)
                    if(feng_value[i]>sub_5_value)
                    {
                        if(feng_value_zu[i]!=max_value_index&&feng_value_zu[i]!=sub_1_index&&feng_value_zu[i]!=sub_2_index&&feng_value_zu[i]!=sub_3_index&&feng_value_zu[i]!=sub_4_index){
                        sub_5_value=feng_value[i];
                        sub_5_index=feng_value_zu[i];
                        }
                    }
                
        //        printf("{sub_1_value}%.1f",sub_1_value);
        //        printf("{sub_2_value}%.1f",sub_2_value);
        //        printf("{sub_3_value}%.1f",sub_3_value);
        //        printf("{sub_4_value}%.1f",sub_4_value);
        //        printf("{sub_5_value}%.1f",sub_5_value);
        //        printf("{max_value}%.1f",max_value);
        //        
        //        printf("{sub_1_index}%d",sub_1_index);
        //        printf("{sub_2_index}%d",sub_2_index);
        //        printf("{sub_3_index}%d",sub_3_index);
        //        printf("{sub_4_index}%d",sub_4_index);
        //        printf("{sub_5_index}%d",sub_5_index);
        //        printf("{max_value_index}%d",max_value_index);
                
                
                if(max_value_index>sub_1_index)
                {
                    if(((sub_2_index>max_value_index)||(sub_2_index<sub_1_index))&&
                        ((sub_3_index>max_value_index)||(sub_3_index<sub_1_index))&&
                        ((sub_4_index>max_value_index)||(sub_4_index<sub_1_index))&&
                        ((sub_5_index>max_value_index)||(sub_5_index<sub_1_index))
                    )
                        danfeng=1;
                    else 
                        danfeng=0;
                        
                }
                else if(max_value_index<sub_1_index)
                {
                    if(((sub_2_index>sub_1_index)||(sub_2_index<max_value_index))&&
                        ((sub_3_index>sub_1_index)||(sub_3_index<max_value_index))&&
                        ((sub_4_index>sub_1_index)||(sub_4_index<max_value_index))&&
                        ((sub_5_index>sub_1_index)||(sub_5_index<max_value_index))
                    )
                        danfeng=1;
                    else 
                        danfeng=0;
                        
                }
                //printf("{danfeng}%d\r\n",danfeng);
                if(danfeng==1)
                {
                    if(((max_value-sub_1_value)>(sub_1_value-sub_2_value))&&((sub_1_value-sub_2_value)<(sub_2_value-sub_3_value)))
                    {
                        if((sub_2_value+sub_3_value)/2<(max_value+sub_1_value)/4)
                            mode=ASK;
                    }
                    else if((max_value-sub_1_value)<(sub_1_value-sub_2_value))
                    {
                        if((Amplitude_Data[(max_value_index+sub_1_index)/2]+Amplitude_Data[(max_value_index+sub_1_index)/2-1]+Amplitude_Data[(max_value_index+sub_1_index)/2+1])/3<(max_value+sub_1_value)/6)
                            mode=PSK;
                        else
                        {
                            if((sub_2_value+sub_3_value)/2<(max_value+sub_1_value)/4)
                                mode=ASK;
                        }
                           
                    }
                    else
                    {   
                        mode=ASK;
                    }
                }
                else
                {
                        mode=FSK;
                }
                
                mode_zu[count%100]=mode;
                if(++count==100) count=0;
                for(int i=0;i<100;i++)
                {
                    if(mode_zu[i]==ASK)
                        count_ASK++;
                    else if(mode_zu[i]==PSK)
                        count_PSK++;
                    else if(mode_zu[i]==FSK)
                        count_FSK++;
                }
                if(count_ASK>count_PSK&&count_ASK>count_FSK)
                {
                    mode=ASK;
                }
                else
                {
                    if(count_FSK>count_PSK)
                        mode=FSK;
                    else 
                        mode=PSK;
                    
                    if(count_FSK>40&&count_PSK>40)
                        mode=PSK;

                }
                
//                printf("{count_FSK}%d\r\n",count_FSK);
//                printf("{count_ASK}%d\r\n",count_ASK);
//                printf("{count_PSK}%d\r\n",count_PSK);
//                switch(mode)
//                {
//                    case ASK:
//                        printf("{MODE}ASK\r\n");
//                        HMI_printf("\x01");
//                        pwm_flag=0;
//                    break;
//                    case FSK:
//                        printf("{MODE}FSK\r\n");
//                        HMI_printf("\x02");
//                        pwm_flag=0;
//                    break;
//                    case PSK:
//                        printf("{MODE}PSK\r\n");
//                        HMI_printf("\x03");
//                        pwm_flag=1;
//                    break;
//                }
                
                memset(feng_value,0,400);
                memset(feng_value_zu,0,400);
                max_value=0;
                sub_1_value=0,sub_2_value=0,sub_3_value=0,sub_4_value=0,sub_5_value=0;
                count_ASK=0,count_PSK=0,count_FSK=0;
            }
            switch(mode)
            {
                case ASK:
//                    printf("{MODE}ASK\r\n");
                    HMI_printf("\x01");
                    pwm_flag=0;
                break;
                case FSK:
//                    printf("{MODE}FSK\r\n");
                    HMI_printf("\x02");
                    pwm_flag=0;
                break;
                case PSK:
//                    printf("{MODE}PSK\r\n");
                    HMI_printf("\x03");
                    pwm_flag=1;
                break;
            }
            start_flag = 0;
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB, LED0_Pin, 0);
        }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//while(1);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 288;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief 串口接收回调(IDLE)
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart == &huart2) {
		HMI_RX.Size = Size;
		HMI_RX.Flag = 1;
		
		HAL_UARTEx_ReceiveToIdle_IT(&huart2, HMI_RX.Buffer, BUFFER_SIZE);
	}

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
