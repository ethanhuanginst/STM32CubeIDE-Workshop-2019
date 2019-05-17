
#include <math.h>
#include <stdlib.h>
#include <memory.h>

#include "stm32f7xx_hal.h"
#include "GUI.h"
#include "spectrogram.h"
#include "adc.h"
#include "options.h"


spectrogram_t *spectrogram_default() {
    spectrogram_t *s = malloc(sizeof(spectrogram_t));

    s->pos_x = 30;
    s->pos_y = 5;

    s->graticules_nx = 10;
    s->graticules_ny = 8;

    s->graticule_start_y = 0;
    s->graticule_step_y = -10;

    s->size_x = 360;
    s->size_y = 225;

    s->history_readings = 20;
    s->history_position = 0;

    s->npoints = s->size_x;

    spectrogram_init_data(s);

    return s;
}


/* Colourmapping code from:
 * https://stackoverflow.com/questions/7706339/grayscale-to-red-green-blue-matlab-jet-color-scale
 */

double interpolate( double val, double y0, double x0, double y1, double x1 ) {
    return (val-x0)*(y1-y0)/(x1-x0) + y0;
}

double base( double val ) {
    if ( val <= -0.75 ) return 0;
    else if ( val <= -0.25 ) return interpolate( val, 0.0, -0.75, 1.0, -0.25 );
    else if ( val <= 0.25 ) return 1.0;
    else if ( val <= 0.75 ) return interpolate( val, 1.0, 0.25, 0.0, 0.75 );
    else return 0.0;
}

double red( double gray ) {
    return base( gray - 0.5 );
}
double green( double gray ) {
    return base( gray );
}
double blue( double gray ) {
    return base( gray + 0.5 );
}

#define LUT_ELEMENTS 64
uint32_t lut[LUT_ELEMENTS] = {0};

void lut_init() {
    for(int i = 0; i != LUT_ELEMENTS; ++i) {
        float gray = (float)i / (float)LUT_ELEMENTS;
        uint32_t colour =
            (uint8_t)(red(gray)*255) |
            (uint8_t)(green(gray)*255) << 8 |
            (uint8_t)(blue(gray)*255) << 16;
        lut[i] = colour;
    }
}

uint32_t lut_lookup(uint8_t v) {
    return lut[v/(256/LUT_ELEMENTS)];
}

void spectrogram_init_data(spectrogram_t *s) {
    lut_init();

    s->data = malloc(s->npoints * sizeof(int16_t));

    s->data_normal = malloc(s->npoints * sizeof(int16_t));

    for(int i = 0; i != s->npoints; ++i) {
        s->data_normal[i] = 0;
    }

    s->data_history = malloc(s->history_readings * sizeof(uint16_t*));
    for( int i = 0; i != s->history_readings; ++i ) {
        s->data_history[i] = malloc(s->npoints * sizeof(uint16_t));
    }
}

void spectrogram_draw(spectrogram_t* s) {

    if(option_get_selection(OPTION_ID_VIEW_WATERFALL) == OPTION_ID_VIEW_WATERFALL_ON) {

        for( int i = 0; i != s->npoints; ++i) {
            s->data_history[s->history_position][i] = s->data[i];
        }
        s->history_position = (++s->history_position) % s->history_readings;

        for( int i = 0; i != s->history_readings; ++i) {
            for( int j = 0; j < s->npoints; j += 3) {
                GUI_SetColor(lut_lookup(s->data_history[i][j])*(255/s->size_y));

                int i_up = i - s->history_position;

                if(i_up < 0) {
                    i_up += s->history_readings;
                }

                GUI_FillRect( s->pos_x + j,
                              s->pos_y + i_up * (s->size_y / s->history_readings),
                              s->pos_x + j + 3,
                              s->pos_y + (i_up+1) * (s->size_y / s->history_readings));
            }
        }

    }

    GUI_SetColor(GUI_DARKGRAY);

    for( int i = 0; i != s->graticules_nx; ++i ) {
        GUI_DrawVLine(s->pos_x + i * s->size_x/s->graticules_nx, s->pos_y, s->pos_y+s->size_y);
        for( int k = 0; k != 5; ++k) {
            GUI_DrawVLine(s->pos_x + i * s->size_x/s->graticules_nx
                          + k * 0.2*s->size_x/s->graticules_nx,
                          s->pos_y+s->size_y/2-3,
                          s->pos_y+s->size_y/2+3);
        }
    }

    for( int i = 0; i != s->graticules_ny; ++i ) {
        GUI_DrawHLine(s->pos_y + i * s->size_y/s->graticules_ny, s->pos_x, s->pos_x+s->size_x);
        for( int k = 0; k != 5; ++k) {
            GUI_DrawHLine(s->pos_y + i * s->size_y/s->graticules_ny
                          + k * 0.2*s->size_y/s->graticules_ny,
                          s->pos_x+s->size_x/2-3,
                          s->pos_x+s->size_x/2+3);
        }
    }

    GUI_SetColor(GUI_WHITE);

    GUI_DrawRoundedFrame(s->pos_x, s->pos_y, s->size_x+s->pos_x, s->size_y+s->pos_y, 3, 2);

    if(option_get_selection(OPTION_ID_VIEW_WATERFALL) == OPTION_ID_VIEW_WATERFALL_OFF) {

        GUI_SetColor(GUI_YELLOW);

        GUI_DrawGraph( s->data, s->npoints, s->pos_x, s->pos_y );

    }

    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font8_ASCII);

    char buf[16];
    for( int i = 0; i != s->graticules_ny+1; ++i ) {
        snprintf(buf, 32, "%d",(int)(s->graticule_start_y + i*s->graticule_step_y));
        GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_VCENTER);
        GUI_DispStringAt( buf, -3 + s->pos_x, s->pos_y + i * s->size_y/s->graticules_ny );
    }
}

void spectrogram_fake_data(spectrogram_t* s) {
}
