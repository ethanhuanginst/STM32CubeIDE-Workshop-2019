#ifndef ADC_H
#define ADC_H

#include "stm32f7xx_hal.h"
#include "spectrogram.h"

int adc_init();
void adc_set_spectrogram(spectrogram_t *s);

#endif
