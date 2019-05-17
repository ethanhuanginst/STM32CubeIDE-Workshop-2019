#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <stdint.h>

typedef struct _spectrogram {
    int16_t pos_x;
    int16_t pos_y;

    int16_t size_x;
    int16_t size_y;

    int16_t graticules_nx;
    int16_t graticules_ny;

    float graticule_start_y;
    float graticule_step_y;

    int16_t npoints;

    int16_t history_readings;
    int16_t history_position;

    int16_t *data;
    int16_t *data_normal;

    uint16_t **data_history;
} spectrogram_t;

spectrogram_t *spectrogram_default();

void spectrogram_init_data(spectrogram_t *s);

void spectrogram_draw(spectrogram_t* s);

void spectrogram_fake_data(spectrogram_t* s);

#endif
