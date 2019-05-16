/**
  ******************************************************************************
  * @file    STM32F746_DSPDEMO\Src\fft_processing.c
  * @author  MCD Application Team
  * @brief   FFT calculation Service Routines
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

#include "fft.h"
#include "display.h"

float32_t aFFT_Input_f32[FFT_Length_Tab*2];
float32_t aFFT_Output_f32 [FFT_Length_Tab];

float32_t FFT_Input_Q15_f[FFT_Length_Tab*2];
q15_t aFFT_Input_Q15[FFT_Length_Tab*2];
q15_t FFT_Output_Q15[FFT_Length_Tab];

float32_t FFT_Input_Q31_f[FFT_Length_Tab*2];
q31_t aFFT_Input_Q31[FFT_Length_Tab*2];
q31_t FFT_Output_Q31[FFT_Length_Tab];

/**
  * @brief  This function Calculate FFT in Q15.
  * @param  FFT Length : 1024, 256, 64
  * @retval None
  */
void FFT_PROCESSING_Q15Process(uint32_t FFT_Length)
{

  arm_cfft_radix4_instance_q15  FFT_Q15_struct;

  q15_t maxValue;    /* Max FFT value is stored here */
  uint32_t maxIndex;    /* Index in Output array where max value is */

  uint32_t index_fill_input_buffer, index_fill_output_buffer, index_fill_adc_buffer = 0;
  uint32_t duration_us = 0x00;
  for (index_fill_adc_buffer = 0; index_fill_adc_buffer < FFT_Length*2; index_fill_adc_buffer ++)
  {
    aADC1ConvertedValue_s[index_fill_adc_buffer] =uhADCxConvertedValue;

    TIM2_Config();
  }
  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer += 2)
  {
    FFT_Input_Q15_f[(uint16_t)index_fill_input_buffer] = (float32_t)uhADCxConvertedValue / (float32_t)4096.0;
    /* Imaginary part */
    FFT_Input_Q15_f[(uint16_t)(index_fill_input_buffer + 1)] = 0;

    TIM2_Config();
  }

  arm_float_to_q15((float32_t *)&FFT_Input_Q15_f[0], (q15_t *)&aFFT_Input_Q15[0], FFT_Length*2);

  /* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
  arm_cfft_radix4_init_q15(&FFT_Q15_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);

  TimerCount_Start();
  arm_cfft_radix4_q15(&FFT_Q15_struct, aFFT_Input_Q15);
  TimerCount_Stop(nb_cycles);

  GUI_Clear();
  LCD_OUTPUT_Cycles(50, 260, nb_cycles);
  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
  LCD_OUTPUT_DURATION(220, 260, duration_us);

  /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
  arm_cmplx_mag_q15(aFFT_Input_Q15, FFT_Output_Q15, FFT_Length);

  /* Calculates maxValue and returns corresponding value */
  arm_max_q15(FFT_Output_Q15, FFT_Length, &maxValue, &maxIndex);
  maxValue = 0;

  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer++)
  {
    GRAPH_DATA_YT_SetAlign(aGraph_Data[1], GRAPH_ALIGN_LEFT);
    GRAPH_DATA_YT_MirrorX (aGraph_Data[1], 1);
    GRAPH_DATA_YT_AddValue(aGraph_Data[1], aADC1ConvertedValue_s[index_fill_input_buffer] / 100 + 50);
  }
  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
  {
    GRAPH_DATA_YT_SetAlign(aGraph_Data[0], GRAPH_ALIGN_LEFT);
    GRAPH_DATA_YT_MirrorX (aGraph_Data[0], 1);
    GRAPH_DATA_YT_AddValue(aGraph_Data[0], FFT_Output_Q15[index_fill_output_buffer] / 50 + 10);

  }

}

/**
  * @brief  This function Calculate FFT in F32.
  * @param  FFT Length : 1024, 256, 64
  * @retval None
  */
void FFT_PROCESSING_F32Process(uint32_t FFT_Length)
{
  arm_cfft_radix4_instance_f32  FFT_F32_struct;

  float32_t maxValue;    /* Max FFT value is stored here */
  uint32_t maxIndex;    /* Index in Output array where max value is */

  unsigned index_fill_input_buffer, index_fill_output_buffer, index_fill_adc_buffer = 0;
  uint32_t duration_us = 0x00;

  for (index_fill_adc_buffer = 0; index_fill_adc_buffer < FFT_Length*2; index_fill_adc_buffer ++)
  {
    aADC1ConvertedValue_s[index_fill_adc_buffer] = uhADCxConvertedValue;
    TIM2_Config();
  }
  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer += 2)
  {
    aFFT_Input_f32[(uint16_t)index_fill_input_buffer] = (float32_t)uhADCxConvertedValue / (float32_t)4096.0;
    /* Imaginary part */
    aFFT_Input_f32[(uint16_t)(index_fill_input_buffer + 1)] = 0;
    TIM2_Config();
  }
  /* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
  arm_cfft_radix4_init_f32(&FFT_F32_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);

  TimerCount_Start();
  arm_cfft_radix4_f32(&FFT_F32_struct, aFFT_Input_f32);
  TimerCount_Stop(nb_cycles);

  GUI_Clear();
  LCD_OUTPUT_Cycles(50, 260, nb_cycles);
  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
  LCD_OUTPUT_DURATION(220, 260, duration_us);

  /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
  arm_cmplx_mag_f32(aFFT_Input_f32, aFFT_Output_f32, FFT_Length);

  /* Calculates maxValue and returns corresponding value */
  arm_max_f32(aFFT_Output_f32, FFT_Length, &maxValue, &maxIndex);
  maxValue = 0;

  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer++)
  {
    GRAPH_DATA_YT_SetAlign(aGraph_Data[1], GRAPH_ALIGN_LEFT);
    GRAPH_DATA_YT_MirrorX (aGraph_Data[1], 1);
    GRAPH_DATA_YT_AddValue(aGraph_Data[1], aADC1ConvertedValue_s[index_fill_input_buffer] / 100 + 50);
  }
  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
  {
    GRAPH_DATA_YT_SetAlign(aGraph_Data[0], GRAPH_ALIGN_LEFT);
    GRAPH_DATA_YT_MirrorX (aGraph_Data[0], 1);
    GRAPH_DATA_YT_AddValue(aGraph_Data[0], (aFFT_Output_f32[index_fill_output_buffer]) + 10);
  }
}

/**
  * @brief  This function Calculate FFT in Q31.
  * @param  FFT Length : 1024, 256, 64
  * @retval None
  */
void FFT_PROCESSING_Q31Process(uint32_t FFT_Length)
{
  arm_cfft_radix4_instance_q31  FFT_Q31_struct;

  q31_t maxValue;    /* Max FFT value is stored here */
  uint32_t maxIndex;    /* Index in Output array where max value is */

  uint32_t index_fill_input_buffer, index_fill_output_buffer, index_fill_adc_buffer = 0;
  uint32_t duration_us = 0x00;

  for (index_fill_adc_buffer = 0; index_fill_adc_buffer < FFT_Length*2; index_fill_adc_buffer ++)
  {
    aADC1ConvertedValue_s[index_fill_adc_buffer] = uhADCxConvertedValue;
    TIM2_Config();
  }
  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer += 2)
  {
    FFT_Input_Q31_f[(uint16_t)index_fill_input_buffer] = (float32_t)uhADCxConvertedValue / (float32_t)4096.0;
    /* Imaginary part */
    FFT_Input_Q31_f[(uint16_t)(index_fill_input_buffer + 1)] = 0;

    TIM2_Config();
  }

  arm_float_to_q31((float32_t *)&FFT_Input_Q31_f[0], (q31_t *)&aFFT_Input_Q31[0], FFT_Length*2);

  /* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
  arm_cfft_radix4_init_q31(&FFT_Q31_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);

  TimerCount_Start();
  arm_cfft_radix4_q31(&FFT_Q31_struct, aFFT_Input_Q31);
  TimerCount_Stop(nb_cycles);

  GUI_Clear();
  LCD_OUTPUT_Cycles(50, 260, nb_cycles);
  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
  LCD_OUTPUT_DURATION(220, 260, duration_us);

  /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
  arm_cmplx_mag_q31(aFFT_Input_Q31, FFT_Output_Q31, FFT_Length);

  /* Calculates maxValue and returns corresponding value */
  arm_max_q31(FFT_Output_Q31, FFT_Length, &maxValue, &maxIndex);
  maxValue = 0;

  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer++)
  {
    GRAPH_DATA_YT_SetAlign(aGraph_Data[1], GRAPH_ALIGN_LEFT);
    GRAPH_DATA_YT_MirrorX (aGraph_Data[1], 1);
    GRAPH_DATA_YT_AddValue(aGraph_Data[1], aADC1ConvertedValue_s[index_fill_input_buffer] / 100 + 50);
  }
  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
  {
    GRAPH_DATA_YT_SetAlign(aGraph_Data[0], GRAPH_ALIGN_LEFT);
    GRAPH_DATA_YT_MirrorX (aGraph_Data[0], 1);
    GRAPH_DATA_YT_AddValue(aGraph_Data[0], FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10);

  }
}

/************************ (C) COPYRIGHT STMicroelectronics ************************/
