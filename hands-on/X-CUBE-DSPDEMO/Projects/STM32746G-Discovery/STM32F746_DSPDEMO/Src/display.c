/**
  ******************************************************************************
  * @file    STM32F746_DSPDEMO\Src\display.c
  * @author  MCD Application Team
  * @brief   Display routines
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
#include "display.h"
#include "fir.h"
#include "fft.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint16_t uhADCxConvertedValue  = 0;
uint32_t nb_cycles        = 0x00;
DAC_ChannelConfTypeDef   sConfigDspDemo;
ADC_ChannelConfTypeDef   ConfigAdcDspDemo;
uint16_t aADC1ConvertedValue_s [SAMPLES];

GRAPH_DATA_Handle aGraph_Data[2]; // Array of handles for the GRAPH_DATA objects
GUI_COLOR aGraphColor[] = {GUI_RED, GUI_GREEN}; // Array of colors for the GRAPH_DATA objects
DROPDOWN_Handle _hDropDownScale;

int _Stop = 0;
int IsFloat32 	;
int IsFFT 		;
int LP_or_HP		;

int FFT_SIZE_CHOOSE =1024;
/**
  * @brief  This function Draws Lables on graph.
  * @param  WM_HWIN hWin, int Stage
  * @retval None
  */
static void Labels(WM_HWIN hWin, int Stage)
{
  if (Stage == GRAPH_DRAW_LAST)
  {
    char input[] = "-- Input signal";
    char output[] = "-- Output signal";
    GUI_RECT Rect;
    GUI_RECT RectInvalid;

    GUI_SetFont(&GUI_Font13_ASCII);
    WM_GetInsideRect(&Rect);
    WM_GetInvalidRect(hWin, &RectInvalid);
    GUI_SetColor(GUI_GREEN);
    GUI_DispStringInRectEx(input, &Rect, GUI_TA_LEFT, strlen(input), GUI_ROTATE_0);

    GUI_SetColor(GUI_RED);
    GUI_DispStringInRectEx(output, &Rect, GUI_TA_RIGHT, strlen(output), GUI_ROTATE_0);
  }
}
/**
  * @brief  This function show processing duration.
  * @param  LineX,uint16_t LineY, string_to_display
  * @retval None
  */
void LCD_OUTPUT_DURATION(uint16_t LineX, uint16_t LineY, uint32_t string_to_display)
{
  char tmp[64];
  GUI_SetFont(&GUI_Font10_1);
  GUI_SetColor(GUI_YELLOW);
  sprintf((char*)tmp, "Duration in us:%lu", string_to_display);
  GUI_DispStringAt(tmp, LineX, LineY);
}
/**
  * @brief  This function show processing number of cycles.
  * @param  LineX,uint16_t LineY, string_to_display
  * @retval None
  */
void LCD_OUTPUT_Cycles(uint16_t LineX, uint16_t LineY, uint32_t string_to_display)
{
  char tmp[64];
  GUI_SetFont(&GUI_Font10_1);
  GUI_SetColor(GUI_YELLOW);
  sprintf((char*)tmp, "Cycle Number:%lu", string_to_display);
  GUI_DispStringAt(tmp, LineX, LineY);
}


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
  {

    {
      FRAMEWIN_CreateIndirect, "DSP DEMO",  0                ,   0,   0, 490, 250 , FRAMEWIN_CF_ACTIVE
    },
    { GRAPH_CreateIndirect,     0,                   GUI_ID_GRAPH0    ,   5,   5, 465, 130 },
    { SLIDER_CreateIndirect, "change Freq", GUI_ID_SLIDER0, 5, 135, 465, 20, 0, 0, 0 },
    { RADIO_CreateIndirect, "FFT_FIR", GUI_ID_RADIO2, 50, 160, 60, 50, 0, 6402, 0 },
    { RADIO_CreateIndirect, "F32_Q15_Q31", GUI_ID_RADIO3, 105, 160, 78, 60, 0, 5123, 0 },
    { RADIO_CreateIndirect, "POINTS", GUI_ID_RADIO1, 225, 160, 60, 60, 0, 5123, 0 },
    { RADIO_CreateIndirect, "LPF_HPF", GUI_ID_RADIO4, 270, 160, 100, 45, 0, 5122, 0 },

  };

/**
  * @brief  This function refresh user's choises.
  * @param  None
  * @retval None
  */
static void Refresh(void)
{

  if (IsFFT == FFT_PROCESS)
  {
    switch (IsFloat32)
    {
      case Float32:
        FFT_PROCESSING_F32Process(FFT_SIZE_CHOOSE);
        break;

      case Q15:
        FFT_PROCESSING_Q15Process(FFT_SIZE_CHOOSE);
        break;

      case Q31:
        FFT_PROCESSING_Q31Process(FFT_SIZE_CHOOSE);
        break;

        //default:
    }
  }
  else if (IsFFT == FIR_PROCESS)
  {
    switch (IsFloat32)
    {
      case Float32:
        FIR_PROCESSING_F32Process();
        break;

      case Q15:
        FIR_PROCESSING_Q15Process(LP_or_HP);
        break;

      case Q31:
        FIR_PROCESSING_Q31Process();
        break;

        //default:
    }
  }
  else
  {/* empty else*/
  }
}

/**
* @brief  This function is the callback of the gui inerface any time a widget in the dialog changes program come here.
* @param  None
* @retval None
*/
static void GUI_Callback(WM_MESSAGE * pMsg)
{
  unsigned count_of_graphs;
  WM_HWIN  hDlg;
  WM_HWIN  hItem;
  int      NCode;
  int      Id;
  int      Value = 0;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId)
  {
    case WM_INIT_DIALOG:

      hItem = pMsg->hWin;
      FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      FRAMEWIN_SetFont(hItem, GUI_FONT_8_1);
      hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);

      /* Add graphs */
      for (count_of_graphs = 0; count_of_graphs < GUI_COUNTOF(aGraphColor); count_of_graphs++)
      {
        aGraph_Data[count_of_graphs] = GRAPH_DATA_YT_Create(aGraphColor[count_of_graphs], 2048, 0, 0);
        GRAPH_AttachData(hItem, aGraph_Data[count_of_graphs]);
      }
      /*  Set graph attributes */

      GRAPH_SetGridDistY(hItem, 25);
      GRAPH_SetGridVis(hItem, 1);
      GRAPH_SetGridFixedX(hItem, 1);
      GRAPH_SetUserDraw(hItem, Labels);

      /* Init slider widgets */

      hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER0);
      SLIDER_SetRange(hItem, 4, 20);
      SLIDER_SetValue(hItem, 12);
      SLIDER_SetNumTicks(hItem, 5);

      /* Initialization of 'Points' */
      hItem = WM_GetDialogItem(hDlg, GUI_ID_RADIO1 /* Points */);
      RADIO_SetFont(hItem, &GUI_Font10_ASCII);
      RADIO_SetText(hItem, "1024", 0);
      RADIO_SetText(hItem, "256", 1);
      RADIO_SetText(hItem, "64", 2);
      RADIO_SetValue(hItem, 0);
      RADIO_SetTextColor(hItem, 0x000000FF);
      RADIO_SetFont(hItem, GUI_FONT_13B_ASCII);

      /* Initialization of 'FFT FIR' */
      hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO2);
      RADIO_SetTextColor(hItem, 0x00FF0000);
      RADIO_SetText(hItem, "FIR", 0);
      RADIO_SetText(hItem, "FFT", 1);
      RADIO_SetValue(hItem, 0);
      RADIO_SetFont(hItem, GUI_FONT_20B_1);

      /* Initialization of 'F32 Q15 Q31' */
      hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO3/* F32/Q15/Q31 */);
      RADIO_SetFont(hItem, GUI_FONT_13HB_1);
      RADIO_SetTextColor(hItem, 0x00400080);

      RADIO_SetText(hItem, "Float 32", 0);
      RADIO_SetText(hItem, "Q 15", 1);
      RADIO_SetText(hItem, "Q 31", 2);
      RADIO_SetValue(hItem, 0);

      /* Initialization of 'LPF or HPF Q15' */
      hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO4/* HPF/LPF Q15 */);
      RADIO_SetText(hItem, "LPF Q15", 0);
      RADIO_SetText(hItem, "HPF Q15", 1);
      RADIO_SetFont(hItem, GUI_FONT_13B_1);
      WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO4/* HPF/LPF Q15 */));
      WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO3 /* F32/Q15/Q31 */));
      WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO1  /* Points */));
      WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0/* Change freq */));

      break;
    case WM_NOTIFY_PARENT:

      Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
      NCode = pMsg->Data.v;                 // Notification code
      switch (NCode)
      {
        case WM_NOTIFICATION_VALUE_CHANGED:
          switch (Id)
          {
            case GUI_ID_SLIDER0:
              /* Set horizontal grid spacing */
              Value = SLIDER_GetValue(pMsg->hWinSrc) * 50;
              TIM4_Config(Value);
              DAC1_Ch1_SineWaveConfig();
              break;

            case GUI_ID_RADIO1:  /*  choose FFT points*/
              switch (RADIO_GetValue(pMsg->hWinSrc))
              {
                case 0:
                  FFT_SIZE_CHOOSE = 1024;
                  break;
                case 1:
                  FFT_SIZE_CHOOSE = 256;
                  break;
                case 2:
                  FFT_SIZE_CHOOSE = 64;
                  break;
              }
              break;
            case GUI_ID_RADIO2:  /*  choose FFT or FIR*/
              switch (RADIO_GetValue(pMsg->hWinSrc))
              {
                case 0:
                  IsFFT = FIR_PROCESS;
                  WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0/* Change freq */));
                  WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO1  /* Points */));
                  WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO4/* HPF/LPF Q15 */));
                  WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO3 /* F32/Q15/Q31 */));

                  break;
                case 1:
                  IsFFT = FFT_PROCESS;
                  WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO4/* HPF/LPF Q15 */));
                  WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO3 /* F32/Q15/Q31 */));
                  WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO1  /* Points */));
                  WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0/* Change freq */));
                  break;
              }
              break;
            case GUI_ID_RADIO3:  /*  choose F32 Q15 Q31*/
              switch (RADIO_GetValue(pMsg->hWinSrc))
              {
                case 0:
                  IsFloat32 = Float32;
                  WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO4/* HPF/LPF Q15 */));
                  break;
                case 1:
                  IsFloat32 = Q15;
                  if ((IsFFT == FIR_PROCESS) && (IsFloat32 == Q15))
                  {
                    WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO4/* HPF/LPF Q15 */));
                  }
                  else if (IsFloat32 != Q15)
                  {
                    WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO4/* HPF/LPF Q15 */));
                  }
                  break;
                case 2:
                  IsFloat32 = Q31;
                  WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO4/* HPF/LPF Q15 */));
                  break;
              }
              break;
            case GUI_ID_RADIO4:  /*  choose LPF or HPF for FIR Q15*/
              switch (RADIO_GetValue(pMsg->hWinSrc))
              {
                case 0:
                  LP_or_HP = LPF;
                  break;
                case 1:
                  LP_or_HP = HPF;
                  break;
              }
              break;
          }
          break;
        default:
          WM_DefaultProc(pMsg);
      }
  }
}

/**
  * @brief  Main task
  * @param  None
  * @retval None
  */
void MainTask(void)
{

  WM_HWIN hDlg;
  WM_HWIN hGraph;

  DAC1_Ch1_SineWaveConfig();
  ADC1_Ch4_DMA_Config();

  hGraph = 0;
  WM_SetCreateFlags(WM_CF_MEMDEV);

  GUI_Init();

  /* Check if recommended memory for the sample is available */
  if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY)
  {
    GUI_ErrorOut("Not enough memory available.");
    return;
  }
  /* GUI_CURSOR_Show(); */
  WM_SetDesktopColor(GUI_BLACK);
#if GUI_SUPPORT_MEMDEV
  WM_SetCreateFlags(WM_CF_MEMDEV);
#endif
  hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), GUI_Callback, 0, 0, 0);
  while (1)
  {
#ifdef WIN32
    GUI_Delay(10);
#endif
    if (!_Stop)
    {
      if (!hGraph)
      {
        hGraph = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
      }
      /* Adjust grid */
      GRAPH_SetGridVis  (hGraph, 1);
      GRAPH_SetGridDistX(hGraph, GRID_DIST_X);
      GRAPH_SetGridDistY(hGraph, GRID_DIST_Y);
      WM_BringToBottom  (hGraph);
      Refresh();

    }
    GUI_Exec();
  }
}


/************************ (C) COPYRIGHT STMicroelectronics ************************/
