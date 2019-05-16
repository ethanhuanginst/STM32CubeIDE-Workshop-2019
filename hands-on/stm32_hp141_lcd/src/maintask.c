#include "GUI.h"
#include <math.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_ts.h"
#include "spectrogram.h"
#include "set_frequencyDLG.h"
#include "menuDLG.h"
#include "BUTTON.h"
#include "options.h"
#include "adc.h"

short points[400];

long frameCounter = 0;
short fps = 0;
short up = 0;

void BSP_Pointer_Update(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    fps = frameCounter;
    frameCounter = 0;
    ++up;
}

void MainTask(void) {


    BSP_TS_Init(480, 272);

    options_init();

    spectrogram_t *s = spectrogram_default();

    adc_set_spectrogram(s);

    adc_init();

    BUTTON_SKINFLEX_PROPS Props;
    BUTTON_GetSkinFlexProps(&Props, BUTTON_SKINFLEX_PI_ENABLED);
    Props.aColorFrame[0] = GUI_WHITE;
    Props.aColorFrame[1] = GUI_DARKGRAY;
    Props.aColorFrame[2] = GUI_BLACK;
    Props.aColorUpper[0] = GUI_BLACK;
    Props.aColorUpper[1] = GUI_BLACK;
    Props.aColorLower[0] = GUI_BLACK;
    Props.aColorLower[1] = GUI_BLACK;
    Props.Radius = 6;
    BUTTON_SetSkinFlexProps(&Props, BUTTON_SKINFLEX_PI_ENABLED);
    Props.aColorFrame[0] = GUI_GREEN;
    Props.aColorFrame[1] = GUI_DARKGREEN;
    BUTTON_SetSkinFlexProps(&Props, BUTTON_SKINFLEX_PI_FOCUSSED);
    Props.aColorFrame[2] = GUI_YELLOW;
    Props.aColorUpper[0] = GUI_YELLOW;
    Props.aColorUpper[1] = GUI_YELLOW;
    Props.aColorLower[0] = GUI_YELLOW;
    Props.aColorLower[1] = GUI_YELLOW;
    BUTTON_SetSkinFlexProps(&Props, BUTTON_SKINFLEX_PI_PRESSED);
    BUTTON_SetDefaultTextColor( GUI_WHITE, BUTTON_CI_UNPRESSED);
    BUTTON_SetDefaultTextColor( GUI_WHITE, BUTTON_CI_DISABLED);
    BUTTON_SetDefaultTextColor( GUI_BLACK, BUTTON_CI_PRESSED);

    FRAMEWIN_SetDefaultClientColor( GUI_BLACK );
    FRAMEWIN_SetDefaultTitleHeight(0);
    FRAMEWIN_SetDefaultBorderSize(0);

    GUI_HWIN menu_dialog = Createmenu();
    WM_ShowWindow( menu_dialog );
    WM_SetFocus( menu_dialog );

    while(1) {

        spectrogram_fake_data(s);

        GUI_MULTIBUF_Begin();
        GUI_Clear();

        GUI_SetFont(&GUI_Font8_ASCII);
        char buf[32];
        snprintf(buf, 32, "%dF/%dU", fps, up);
        GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_BOTTOM);
        GUI_DispStringAt( buf, LCD_GetXSize(), LCD_GetYSize());

        spectrogram_draw(s);

        GUI_SetFont(&GUI_Font8x15B_ASCII);
        GUI_SetTextAlign( GUI_TA_LEFT | GUI_TA_BOTTOM);
        GUI_DispStringAt( "START: 10.00 MHz", 5, LCD_GetYSize()-16);
        GUI_DispStringAt( "STOP:  2.000 GHz", 5, LCD_GetYSize());
        GUI_DispStringAt( "REF.LVL: 0 dBm", 153, LCD_GetYSize()-16);
        GUI_DispStringAt( "RBW: 300.0 KHz", 153, LCD_GetYSize());
        GUI_DispStringAt( "MIX.MODE: 1+", 285, LCD_GetYSize()-16);
        GUI_DispStringAt( "ST: 10.00 mS", 285, LCD_GetYSize());

        BSP_Pointer_Update();
        GUI_Exec();

        GUI_MULTIBUF_End();
        ++frameCounter;
    }
}

void BSP_Pointer_Update(void) {
    GUI_PID_STATE TS_State;
    static TS_StateTypeDef prev_state;
    TS_StateTypeDef  ts;
    uint16_t xDiff, yDiff;
    BSP_TS_GetState(&ts);
    TS_State.Pressed = ts.touchDetected;

    xDiff = (prev_state.touchX[0] > ts.touchX[0]) ?
            (prev_state.touchX[0] - ts.touchX[0]) :
            (ts.touchX[0] - prev_state.touchX[0]);

    yDiff = (prev_state.touchY[0] > ts.touchY[0]) ?
            (prev_state.touchY[0] - ts.touchY[0]) :
            (ts.touchY[0] - prev_state.touchY[0]);

    if((prev_state.touchDetected != ts.touchDetected )||
            (xDiff > 3 )||
            (yDiff > 3))
    {
        prev_state.touchDetected = ts.touchDetected;

        if((ts.touchX[0] != 0) &&  (ts.touchY[0] != 0)) 
        {
            prev_state.touchX[0] = ts.touchX[0];
            prev_state.touchY[0] = ts.touchY[0];
        }

        TS_State.Layer = 0;
        TS_State.x = prev_state.touchX[0];
        TS_State.y = prev_state.touchY[0];

        GUI_TOUCH_StoreStateEx(&TS_State);
    }
}
