/**
  ******************************************************************************
  * @file    STM32F746_DSPDEMO\Src\global.c
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    09-October-2015
  * @brief   Global & common Service Routines.
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
/* Includes ------------------------------------------------------------------*/
#include "global.h"

/* ----------------------------------------------------------------------
** Sine input signal
** ------------------------------------------------------------------- */

const uint16_t Sine12bit[NB_SAMPLES] =
  {
    2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
    3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
    599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647,
    2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
    3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
    599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647
  };
/**
  * @brief  DAC  Channel1 SineWave Configuration
  * @param  None
  * @retval None
  */
void DAC1_Ch1_SineWaveConfig(void)
{
  DacHandleDspDemo.Instance = DAC;
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandleDspDemo) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  sConfigDspDemo.DAC_Trigger = DAC_TRIGGER_T4_TRGO;
  sConfigDspDemo.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandleDspDemo, &sConfigDspDemo, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-2- Enable DAC selected channel and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandleDspDemo, DACx_CHANNEL, (uint32_t*)Sine12bit, NB_SAMPLES, DAC_ALIGN_12B_R) != HAL_OK)
  {
    /* Start DMA Error */
    Error_Handler();
  }

}

/**
  * @brief  ADC1 channel4 with DMA configuration
  * @param  None
  * @retval None
  */
void ADC1_Ch4_DMA_Config(void)
{
  /*##-1- Configure the ADC peripheral #######################################*/
  AdcHandleDspDemo.Instance          = ADCx;

  AdcHandleDspDemo.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
  AdcHandleDspDemo.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandleDspDemo.Init.ScanConvMode = DISABLE;
  AdcHandleDspDemo.Init.ContinuousConvMode = DISABLE;
  AdcHandleDspDemo.Init.DiscontinuousConvMode = DISABLE;
  AdcHandleDspDemo.Init.NbrOfDiscConversion = 0;
  AdcHandleDspDemo.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_FALLING;
  AdcHandleDspDemo.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
  AdcHandleDspDemo.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandleDspDemo.Init.NbrOfConversion = 1;
  AdcHandleDspDemo.Init.DMAContinuousRequests = ENABLE;
  AdcHandleDspDemo.Init.EOCSelection = DISABLE;

  if (HAL_ADC_Init(&AdcHandleDspDemo) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure ADC regular channel ######################################*/
  ConfigAdcDspDemo.Channel = ADCx_CHANNEL;
  ConfigAdcDspDemo.Rank = 1;
  ConfigAdcDspDemo.SamplingTime = ADC_SAMPLETIME_56CYCLES;
  ConfigAdcDspDemo.Offset = 0;

  if (HAL_ADC_ConfigChannel(&AdcHandleDspDemo, &ConfigAdcDspDemo) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /*##-3- Start the conversion process and enable interrupt ##################*/
  if (HAL_ADC_Start_DMA(&AdcHandleDspDemo, (uint32_t*)&uhADCxConvertedValue, 1) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
}


/**
  * @brief  TIM2 Configuration
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{
  static TIM_HandleTypeDef htim;
  TIM_MasterConfigTypeDef  sMasterConfig;
  TIM_OC_InitTypeDef  TIM_OCInitStructure;
  uint32_t T2PrescalerValue = 0;

  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
  htim.Instance = TIM2;
  T2PrescalerValue = (uint32_t) ((SystemCoreClock / 2) / 1000) - 1;
  htim.Init.Period = 500 - 1;
  htim.Init.Prescaler = T2PrescalerValue;
  htim.Init.ClockDivision = 0;
  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&htim);
  HAL_TIM_OC_Init(&htim);
  TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM2;
  TIM_OCInitStructure.Pulse = (htim.Init.Period - 1) / 2;
  TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_LOW;
  TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  TIM_OCInitStructure.OCFastMode =  TIM_OCFAST_ENABLE;
  TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
  TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  HAL_TIM_OC_ConfigChannel(&htim, &TIM_OCInitStructure, TIM_CHANNEL_2);

  /* TIM2 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE ;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  /* Enable TIM peripheral counter */
  HAL_TIM_Base_Start(&htim);
  HAL_TIM_OC_Start(&htim, TIM_CHANNEL_2);
}
/**
  * @brief  TIM4 Configuration
  * @param  None
  * @retval None
  */
void TIM4_Config(uint32_t Period)
{
  static TIM_HandleTypeDef htim;
  TIM_MasterConfigTypeDef  sMasterConfig;

  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
  htim.Instance = TIM4;

  htim.Init.Period = Period + 1;
  htim.Init.Prescaler = 0;
  htim.Init.ClockDivision = 0;
  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Base_Init(&htim);

  /* TIM4 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&htim);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while (1)
  {}
}
/************************ (C) COPYRIGHT STMicroelectronics ************************/

