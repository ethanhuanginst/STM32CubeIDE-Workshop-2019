/**
  ******************************************************************************
  * @file    STM32F746_DSPDEMO\Inc\global.h 
  * @author  MCD Application Team
  * @brief   Header for global.c module
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H
#define __FPU_PRESENT             1       /*!< FPU present */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rtc.h"

#include "stm32746g_discovery.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_sdram.h"
#include "gui.h"
#include "dialog.h"
#include "rtc.h"
#include "calibration.h"

#include <stdlib.h>
#include <math.h>
#include "arm_math.h"
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include "arm_math.h"
/* Private defines -----------------------------------------------------------*/
#define DACx                            DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA1_CLK_ENABLE()

#define DACx_CLK_ENABLE()               __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_FORCE_RESET()              __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC_RELEASE_RESET()

/* Definition for DACx Channel Pin */
#define DACx_CHANNEL_PIN                GPIO_PIN_4
#define DACx_CHANNEL_GPIO_PORT          GPIOA

/* Definition for DACx's Channel */
#define DACx_DMA_CHANNEL                DMA_CHANNEL_7

#define DACx_CHANNEL                    DAC_CHANNEL_1

/* Definition for DACx's DMA_STREAM */
#define DACx_DMA_INSTANCE               DMA1_Stream5

/* Definition for DACx's NVIC */
#define DACx_DMA_IRQn                   DMA1_Stream5_IRQn
#define DACx_DMA_IRQHandler             DMA1_Stream5_IRQHandler

/* User can use this section to tailor ADCx instance used and associated 
   resources */
/* Definition for ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define DMA2_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()     
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
     
#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_4
#define ADCx_CHANNEL_GPIO_PORT          GPIOA

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_4

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                DMA_CHANNEL_0
#define ADCx_DMA_STREAM                 DMA2_Stream0 

//
// Recommended memory to run the sample with adequate performance
//
#define RECOMMENDED_MEMORY (1024L * 20)
#define GRID_DIST_X      	25
#define GRID_DIST_Y      	10

/* defines number of us per second */
#define US_IN_SECOND  					((uint32_t)1000000)

/* Systick Start */
#define TimerCount_Start()	do{\
							SysTick->LOAD  =  0xFFFFFF  ;	/* set reload register */\
							SysTick->VAL  =  0  ;			/* Clear Counter */		 \
							SysTick->CTRL  =  0x5 ;			/* Enable Counting*/	 \
							}while(0)

/* Systick Stop and retrieve CPU Clocks count */								
#define TimerCount_Stop(Value) 	do {\
								SysTick->CTRL  =0;	/* Disable Counting */				 \
								Value = SysTick->VAL;/* Load the SysTick Counter Value */\
								Value = 0xFFFFFF - Value;/* Capture Counts in CPU Cycles*/\
								}while(0)
#define FIR_PROCESS 0
#define FFT_PROCESS 1

#define Float32 0
#define Q15 		1
#define Q31 		2
#define LPF			0
#define HPF			1
	
#define FFT_INVERSE_FLAG        ((uint8_t)0)
#define FFT_Normal_OUTPUT_FLAG  ((uint8_t)1)
#define BUFFER_SIZE    		240
#define NB_SAMPLES     		64
#define SAMPLES						2048 			/* 256 real party and 256 imaginary parts */

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void DAC1_Ch1_SineWaveConfig(void);
void ADC1_Ch4_DMA_Config(void);
void TIM2_Config(void);
void Error_Handler(void);
void TIM4_Config(uint32_t);

void LCD_OUTPUT_Cycles(uint16_t,uint16_t, uint32_t);
void LCD_OUTPUT_DURATION(uint16_t,uint16_t, uint32_t);

/* External variables --------------------------------------------------------*/
extern uint16_t aADC1ConvertedValue_s [SAMPLES];
extern uint16_t uhADCxConvertedValue  ;
extern uint32_t nb_cycles 						;

extern ADC_ChannelConfTypeDef 		ConfigAdcDspDemo;
extern ADC_HandleTypeDef    			AdcHandleDspDemo;
extern GPIO_InitTypeDef     			GPIO_InitStructDspDemo;
extern DAC_HandleTypeDef    			DacHandleDspDemo;
extern DAC_ChannelConfTypeDef 		sConfigDspDemo;
extern TIM_HandleTypeDef    Tim3HandleDspDemo;

#endif /* __GLOBAL_H */
/************************ (C) COPYRIGHT STMicroelectronics ************************/
