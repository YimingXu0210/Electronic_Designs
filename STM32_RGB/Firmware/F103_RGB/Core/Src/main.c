/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "./WS2812/WS2812.h"
#include "./INA226/INA226.h"
//#include "./ST7789/ST7789.h"


#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_DMA_CHN		3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t Adc_1_V[ADC_DMA_CHN];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float BSP_Get_Temperature(float Rntc)
{
	float N1,N2,N3,N4;
	float BX = 3950;
	float T25 = 298.15;
	float R25 = 10;
	N1 = (log(Rntc)-log(R25))/BX;
	N2 = 1/T25 + N1;
	N3 = 1/N2;
	N4 = N3-273.15;
	return N4;
}

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
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	INA226_init();
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)Adc_1_V, ADC_DMA_CHN);
	
	WS2812_SetColors_All(0);

	printf("STM32F103CBT6\r\n");

		uint32_t colors[Led_Num] = {
		0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
		0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
		
		0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
		0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
		
		0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
		0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
		
		0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
		0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF
	};
	
		uint32_t colors1[Led_Num] = {
		0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
		0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
		
		0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
		0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
		
		0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
		0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
		
		0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
		0x000000, 0x000000, 0x000000, 0x000000, 0x000000
	};
		
		uint32_t colors2[Led_Num] = {
		0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
		0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
		
		0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
		0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
		
		0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
		0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
		
		0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
		0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF
	};

  
	// 初始化LCD
	
	// 在初始化前添加SPI回环测试
uint8_t test_data = 0xAA;
uint8_t received = 0;
HAL_SPI_TransmitReceive(&hspi1, &test_data, &received, 1, 100);
if(received != test_data) {
	printf("err\r\n");
}
	
//	ST7789_Init(&hspi1);
	TIM2->PSC = 72-1;
	TIM2->ARR = 1000-1;
	TIM2->CCR4 = 1000;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
    
//    // 填充屏幕测试
//    ST7789_FillScreen(RED);
//    HAL_Delay(1000);
//    ST7789_FillScreen(GREEN);
//    HAL_Delay(1000);
//    ST7789_FillScreen(BLUE);
//    HAL_Delay(1000);
//    
//    // 绘制测试图形
//    for(int i = 0; i < 100; i++) {
//        ST7789_DrawPixel(i, i, WHITE);
//    }
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		WS2812_SetColors(colors1);
//		HAL_Delay(100);
//		WS2812_SetColors(colors2);
		HAL_Delay(1000);
	  
//		ST7789_FillScreen(RED);
	  
		float tmp = Adc_1_V[1] * 3.3 / 4096.;
		float tmp1 = (3.3 * 10 - 10 * tmp) / tmp;

		printf("V:%.5f\tI:%.5f\tP:%.5f\tBat:%.5f\tNTC:%.5f\r\n",
			INA226_GetBusV(),
			INA226_GetCurrent(),
			INA226_GetPower(),
			Adc_1_V[0] * 3.3 * 1.625 / 4096.,
			BSP_Get_Temperature(tmp1));
	  
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
