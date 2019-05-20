/**
  ******************************************************************************
  * @file    STM32F746_DSPDEMO\Src\fir_processing.c
  * @author  MCD Application Team
  * @brief   Applying FIR Service Routines
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
#include "fir.h"
#include "display.h"

/* ----------------------------------------------------------------------
** Test input signal contains 1000Hz + 15000 Hz
** ------------------------------------------------------------------- */
float32_t aFIR_F32_1kHz_15kHz[TEST_LENGTH_SAMPLES] =
{
+0.0000000000f, +0.5924659585f, -0.0947343455f, +0.1913417162f, +1.0000000000f, +0.4174197128f, +0.3535533906f, +1.2552931065f,
+0.8660254038f, +0.4619397663f, +1.3194792169f, +1.1827865776f, +0.5000000000f, +1.1827865776f, +1.3194792169f, +0.4619397663f,
+0.8660254038f, +1.2552931065f, +0.3535533906f, +0.4174197128f, +1.0000000000f, +0.1913417162f, -0.0947343455f, +0.5924659585f,
-0.0000000000f, -0.5924659585f, +0.0947343455f, -0.1913417162f, -1.0000000000f, -0.4174197128f, -0.3535533906f, -1.2552931065f,
-0.8660254038f, -0.4619397663f, -1.3194792169f, -1.1827865776f, -0.5000000000f, -1.1827865776f, -1.3194792169f, -0.4619397663f,
-0.8660254038f, -1.2552931065f, -0.3535533906f, -0.4174197128f, -1.0000000000f, -0.1913417162f, +0.0947343455f, -0.5924659585f,
+0.0000000000f, +0.5924659585f, -0.0947343455f, +0.1913417162f, +1.0000000000f, +0.4174197128f, +0.3535533906f, +1.2552931065f,
+0.8660254038f, +0.4619397663f, +1.3194792169f, +1.1827865776f, +0.5000000000f, +1.1827865776f, +1.3194792169f, +0.4619397663f,
+0.8660254038f, +1.2552931065f, +0.3535533906f, +0.4174197128f, +1.0000000000f, +0.1913417162f, -0.0947343455f, +0.5924659585f,
+0.0000000000f, -0.5924659585f, +0.0947343455f, -0.1913417162f, -1.0000000000f, -0.4174197128f, -0.3535533906f, -1.2552931065f,
-0.8660254038f, -0.4619397663f, -1.3194792169f, -1.1827865776f, -0.5000000000f, -1.1827865776f, -1.3194792169f, -0.4619397663f,
-0.8660254038f, -1.2552931065f, -0.3535533906f, -0.4174197128f, -1.0000000000f, -0.1913417162f, +0.0947343455f, -0.5924659585f,
+0.0000000000f, +0.5924659585f, -0.0947343455f, +0.1913417162f, +1.0000000000f, +0.4174197128f, +0.3535533906f, +1.2552931065f,
+0.8660254038f, +0.4619397663f, +1.3194792169f, +1.1827865776f, +0.5000000000f, +1.1827865776f, +1.3194792169f, +0.4619397663f,
+0.8660254038f, +1.2552931065f, +0.3535533906f, +0.4174197128f, +1.0000000000f, +0.1913417162f, -0.0947343455f, +0.5924659585f,
+0.0000000000f, -0.5924659585f, +0.0947343455f, -0.1913417162f, -1.0000000000f, -0.4174197128f, -0.3535533906f, -1.2552931065f,
-0.8660254038f, -0.4619397663f, -1.3194792169f, -1.1827865776f, -0.5000000000f, -1.1827865776f, -1.3194792169f, -0.4619397663f,
-0.8660254038f, -1.2552931065f, -0.3535533906f, -0.4174197128f, -1.0000000000f, -0.1913417162f, +0.0947343455f, -0.5924659585f,
-0.0000000000f, +0.5924659585f, -0.0947343455f, +0.1913417162f, +1.0000000000f, +0.4174197128f, +0.3535533906f, +1.2552931065f,
+0.8660254038f, +0.4619397663f, +1.3194792169f, +1.1827865776f, +0.5000000000f, +1.1827865776f, +1.3194792169f, +0.4619397663f,
+0.8660254038f, +1.2552931065f, +0.3535533906f, +0.4174197128f, +1.0000000000f, +0.1913417162f, -0.0947343455f, +0.5924659585f,
-0.0000000000f, -0.5924659585f, +0.0947343455f, -0.1913417162f, -1.0000000000f, -0.4174197128f, -0.3535533906f, -1.2552931065f,
-0.8660254038f, -0.4619397663f, -1.3194792169f, -1.1827865776f, -0.5000000000f, -1.1827865776f, -1.3194792169f, -0.4619397663f,
-0.8660254038f, -1.2552931065f, -0.3535533906f, -0.4174197128f, -1.0000000000f, -0.1913417162f, +0.0947343455f, -0.5924659585f,
+0.0000000000f, +0.5924659585f, -0.0947343455f, +0.1913417162f, +1.0000000000f, +0.4174197128f, +0.3535533906f, +1.2552931065f,
+0.8660254038f, +0.4619397663f, +1.3194792169f, +1.1827865776f, +0.5000000000f, +1.1827865776f, +1.3194792169f, +0.4619397663f,
+0.8660254038f, +1.2552931065f, +0.3535533906f, +0.4174197128f, +1.0000000000f, +0.1913417162f, -0.0947343455f, +0.5924659585f,
+0.0000000000f, -0.5924659585f, +0.0947343455f, -0.1913417162f, -1.0000000000f, -0.4174197128f, -0.3535533906f, -1.2552931065f,
-0.8660254038f, -0.4619397663f, -1.3194792169f, -1.1827865776f, -0.5000000000f, -1.1827865776f, -1.3194792169f, -0.4619397663f,
-0.8660254038f, -1.2552931065f, -0.3535533906f, -0.4174197128f, -1.0000000000f, -0.1913417162f, +0.0947343455f, -0.5924659585f,
};

/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** ------------------------------------------------------------------- */
float32_t aFIR_F32_Coeffs[NUM_TAPS] = {
-0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
-0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
+0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
+0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
};
/* ow pass at 1KHz with 40dB at 1.5KHz for SR=16KHz.*/

q15_t aFIR_Q15_Coeffs_LP[NUM_FIR_TAPS_Q15] = {
-217	,   40	,  120,  237,  366,  475,  527,  490,  346,
100		, -217	, -548, -818, -947, -864, -522,   86,  922,
1904	, 2918	, 3835, 4529, 4903, 4903, 4529, 3835, 2918,
1904	,  922	,   86, -522, -864, -947, -818, -548, -217,
100		,  346	,  490,  527,  475,  366,  237,  120,   40,
-217	,    0	,    0,    0,    0,    0,    0,    0,    0,
0,    0};
/* high pass at 1.5KHz with 40dB at 1KHz for SR=16KHz */

q15_t aFIR_Q15_Coeffs_HP[NUM_FIR_TAPS_Q15] = {
-654	,  483	,  393,  321,  222,   76, -108, -299, -447,
-501	, -422	, -200,  136,  520,  855, 1032,  953,  558,
-160	,-1148	,-2290,-3432,-4406,-5060,27477,-5060,-4406,
-3432	,-2290	,-1148, -160,  558,  953, 1032,  855,  520,
136		, -200	, -422, -501, -447, -299, -108,   76,  222,
321		,  393	,  483, -654,    0,    0,    0,    0,    0,
0			,    0	,};
/* Private functions ---------------------------------------------------------*/

void FIR_PROCESSING_F32Process(void);
void FIR_PROCESSING_Q31Process(void);
void FIR_PROCESSING_Q15Process(int LP_or_HP);

/* Private variables ---------------------------------------------------------*/

/*==========FIR==========*/
/* ------------------------------------------------------------------
 * Global variables for FIR LPF Example
 * ------------------------------------------------------------------- */

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;

q31_t aFIR_Q31_1kHz_15kHz[TEST_LENGTH_SAMPLES];
q15_t aFIR_Q15_1kHz_15kHz[TEST_LENGTH_SAMPLES];
/* -------------------------------------------------------------------
 * Declare Test output buffer
 * ------------------------------------------------------------------- */

float32_t 	aFIR_F32_Output[TEST_LENGTH_SAMPLES];
q15_t 			aFIR_Q15_Output[TEST_LENGTH_SAMPLES];
q31_t 			aFIR_Q31_Output[TEST_LENGTH_SAMPLES];

/* -------------------------------------------------------------------
 * Declare State buffer of size (numTaps + blockSize - 1)
 * ------------------------------------------------------------------- */

float32_t 	firStateF32[BLOCK_SIZE + NUM_TAPS - 1];
q31_t 			firStateQ31[BLOCK_SIZE + NUM_TAPS - 1];
q15_t 			firStateQ15[NUM_FIR_TAPS_Q15 + BLOCKSIZE];

q31_t 		aFIR_Q31_Coeffs[NUM_TAPS];

#ifdef TEST_SWV
float32_t swv_aFIR_F32_1kHz_15kHz = 0.0f;
float32_t swv_aFIR_F32_Output = 0.0f;
#endif
/**
  * @brief  This function apply a LP FIR filter in to a F32 data signal.
  * @param  None
  * @retval None
  */
void FIR_PROCESSING_F32Process(void)
{

  arm_fir_instance_f32 FIR_F32_Struct;
  uint32_t counter_FIR_f32_p;
  static int counter_FIR_Ds;
  static int counter_FIR_Dd;
  int DataID = 1;
  uint32_t duration_us = 0x00;
  /* Call FIR init function to initialize the instance structure. */
  arm_fir_init_f32(&FIR_F32_Struct, NUM_TAPS, (float32_t *)&aFIR_F32_Coeffs[0], &firStateF32[0], blockSize);

  /* ----------------------------------------------------------------------
  ** Call the FIR process function for every blockSize samples  
  ** ------------------------------------------------------------------- */
  TimerCount_Start();

  for (counter_FIR_f32_p = 0; counter_FIR_f32_p < numBlocks; counter_FIR_f32_p++)
  {
    arm_fir_f32(&FIR_F32_Struct, aFIR_F32_1kHz_15kHz + (counter_FIR_f32_p * blockSize), aFIR_F32_Output + (counter_FIR_f32_p * blockSize), blockSize);
  }

  TimerCount_Stop(nb_cycles);

  GUI_Clear();
  LCD_OUTPUT_Cycles(50, 260, nb_cycles);
  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
  LCD_OUTPUT_DURATION(220, 260, duration_us);

  GUI_USE_PARA(DataID);
#ifdef TEST_SWV
  swv_aFIR_F32_1kHz_15kHz = aFIR_F32_1kHz_15kHz[counter_FIR_Dd]*25 + 75;
  GRAPH_DATA_YT_AddValue(aGraph_Data[1], swv_aFIR_F32_1kHz_15kHz);
#else
  GRAPH_DATA_YT_AddValue(aGraph_Data[1], (aFIR_F32_1kHz_15kHz[counter_FIR_Dd])*25 + 75);
#endif

  if (++counter_FIR_Dd == GUI_COUNTOF(aFIR_F32_1kHz_15kHz))
  {
    counter_FIR_Dd = 0;
  }

#ifdef TEST_SWV
  swv_aFIR_F32_Output = (aFIR_F32_Output[counter_FIR_Ds])*20 + 25;
  GRAPH_DATA_YT_AddValue(aGraph_Data[0], swv_aFIR_F32_Output);
#else
  GRAPH_DATA_YT_AddValue(aGraph_Data[0], (aFIR_F32_Output[counter_FIR_Ds])*20 + 25);
#endif
  if (++counter_FIR_Ds == GUI_COUNTOF(aFIR_F32_Output))
  {
    counter_FIR_Ds = 0;
  }
}

/**
  * @brief  This function apply a LP/HP FIR filter in to a Q15 data signal.
  * @param  LP or HP
  * @retval None
  */
void FIR_PROCESSING_Q15Process(int LP_or_HP)
{
  uint32_t counter_FIR_Q15_p;
  arm_fir_instance_q15 FIR_Q15_Struct;

  static int counter_FIR_Ds;
  static int counter_FIR_Dd;
  int DataID = 1;
  uint32_t duration_us = 0x00;
  arm_float_to_q15((float32_t *)&aFIR_F32_1kHz_15kHz[0], (q15_t *)&aFIR_Q15_1kHz_15kHz[0], TEST_LENGTH_SAMPLES);
  /* Call FIR init function to initialize the instance structure. */
  if (LP_or_HP == LPF)
  {
    arm_fir_init_q15(&FIR_Q15_Struct, NUM_FIR_TAPS_Q15, aFIR_Q15_Coeffs_LP, firStateQ15, BLOCKSIZE);
  }
  else if (LP_or_HP == HPF)
  {
    arm_fir_init_q15(&FIR_Q15_Struct, NUM_FIR_TAPS_Q15, aFIR_Q15_Coeffs_HP, firStateQ15, BLOCKSIZE);
  }
  else
  {/* empty else */
  }
  TimerCount_Start();

  for (counter_FIR_Q15_p = 0; counter_FIR_Q15_p < numBlocks; counter_FIR_Q15_p++)
  {   // process with FIR
    arm_fir_q15(&FIR_Q15_Struct, aFIR_Q15_1kHz_15kHz + (counter_FIR_Q15_p * BLOCKSIZE), aFIR_Q15_Output + (counter_FIR_Q15_p * BLOCKSIZE), BLOCKSIZE);
  }
  TimerCount_Stop(nb_cycles);

  GUI_Clear();
  LCD_OUTPUT_Cycles(50, 260, nb_cycles);
  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
  LCD_OUTPUT_DURATION(220, 260, duration_us);

  GUI_USE_PARA(DataID);
  GRAPH_DATA_YT_AddValue(aGraph_Data[1], (aFIR_Q15_1kHz_15kHz[counter_FIR_Dd] / 1000) + 75);
  if (++counter_FIR_Dd == GUI_COUNTOF(aFIR_Q15_1kHz_15kHz))
  {
    counter_FIR_Dd = 0;
  }

  GRAPH_DATA_YT_AddValue(aGraph_Data[0], aFIR_Q15_Output[counter_FIR_Ds] / 1350 + 25);
  if (++counter_FIR_Ds == GUI_COUNTOF(aFIR_Q15_Output))
  {
    counter_FIR_Ds = 0;
  }
}


/**
  * @brief  This function apply a LP FIR filter in to a Q31 data signal.
  * @param  None
  * @retval None
  */
void FIR_PROCESSING_Q31Process(void)
{
  uint32_t counter_FIR_Q31_p;
  arm_fir_instance_q31 FIR_Q31_Struct;

  static int counter_FIR_Ds;
  static int counter_FIR_Dd;
  int DataID = 1;
  uint32_t duration_us = 0x00;
  arm_float_to_q31((float32_t *)&aFIR_F32_Coeffs[0], (q31_t *)&aFIR_Q31_Coeffs[0], NUM_TAPS);

  arm_float_to_q31((float32_t *)&aFIR_F32_1kHz_15kHz[0], (q31_t *)&aFIR_Q31_1kHz_15kHz[0], TEST_LENGTH_SAMPLES);
  /* Call FIR init function to initialize the instance structure. */
  arm_fir_init_q31(&FIR_Q31_Struct, NUM_TAPS, (q31_t *)&aFIR_Q31_Coeffs[0], &firStateQ31[0], blockSize);

  /* Call the FIR process function for every blockSize samples  */
  TimerCount_Start();

  for (counter_FIR_Q31_p = 0; counter_FIR_Q31_p < numBlocks; counter_FIR_Q31_p++)
  {
    arm_fir_q31(&FIR_Q31_Struct, aFIR_Q31_1kHz_15kHz + (counter_FIR_Q31_p * blockSize), aFIR_Q31_Output + (counter_FIR_Q31_p * blockSize), blockSize);
  }
  TimerCount_Stop(nb_cycles);

  GUI_Clear();
  LCD_OUTPUT_Cycles(50, 260, nb_cycles);
  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
  LCD_OUTPUT_DURATION(220, 260, duration_us);

  GUI_USE_PARA(DataID);

  GRAPH_DATA_YT_AddValue(aGraph_Data[1], (aFIR_Q31_1kHz_15kHz[counter_FIR_Dd]) / 60092980 + 75);
  if (++counter_FIR_Dd == GUI_COUNTOF(aFIR_Q31_1kHz_15kHz))
  {
    counter_FIR_Dd = 0;
  }

  GRAPH_DATA_YT_AddValue(aGraph_Data[0], aFIR_Q31_Output[counter_FIR_Ds] / 90139470 + 25);
  if (++counter_FIR_Ds == GUI_COUNTOF(aFIR_Q31_Output))
  {
    counter_FIR_Ds = 0;
  }
}
/************************ (C) COPYRIGHT STMicroelectronics ************************/

