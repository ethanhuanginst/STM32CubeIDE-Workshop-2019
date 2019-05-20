#include "adc.h"

/* TEXT DUMP FROM EXAMPLE
 * (a lot of this code was ripped out of the stm32 ADC example)
This example describes how to use the ADC3 and DMA to transfer continuously
converted data from ADC3 to memory.

The ADC3 is configured to convert continuously ADC_CHANNEL_8.

Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC3 DR register to the uhADCxConvertedValue variable.

The uhADCxConvertedValue read value is coded on 12 bits, the Vref+ reference voltage is connected
on the board to VDD (+3.3V), the Vref- reference voltage is connected on the board to the ground.
To convert the read value in volts, here is the equation to apply :
Voltage = uhADCxConvertedValue * (Vref+ - Vref-) / (2^12) = uhADCxConvertedValue * 3.3 / 4096

In this example, the system clock is 216MHz, APB2 = 108MHz and ADC clock = APB2/4.
Since ADC3 clock is 27 MHz and sampling time is set to 3 cycles, the conversion
time to 12bit data is 12 cycles so the total conversion time is (12+3)/27 = 0.56us(1.57Msps).

User can vary the ADC_CHANNEL_8 voltage by applying an input voltage on pin PF10 connected to Arduino CN5 pin A1.
*/

#define ADCx                            ADC3
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_10
#define ADCx_CHANNEL_PIN2               GPIO_PIN_9
#define ADCx_CHANNEL_GPIO_PORT          GPIOF

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_8

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                DMA_CHANNEL_2
#define ADCx_DMA_STREAM                 DMA2_Stream0

/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler             DMA2_Stream0_IRQHandler

ADC_HandleTypeDef    AdcHandle;

#define ADC_BUF_SIZE 2048

volatile uint32_t adc_buffer[ADC_BUF_SIZE] = {0};

int adc_last_conversion_complete = 0;

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void ADCx_DMA_IRQHandler(void) {
  HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
}

void adc_error_handler() {
    while(1){}
}

spectrogram_t *adc_spectrogram = 0;

void adc_set_spectrogram(spectrogram_t *s) {
    adc_spectrogram = s;
}

int adc_init() {
    ADC_ChannelConfTypeDef sConfig;

    AdcHandle.Instance          = ADCx;
    AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
    AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
    /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
    AdcHandle.Init.ScanConvMode          = ENABLE;
    AdcHandle.Init.ContinuousConvMode    = ENABLE;
    /* Continuous mode enabled to have continuous conversion  */
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    /* Parameter discarded because sequencer is disabled */
    AdcHandle.Init.NbrOfDiscConversion   = 0;
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    /* Conversion start trigged at each external event */
    AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion       = 2;
    AdcHandle.Init.DMAContinuousRequests = ENABLE;
    AdcHandle.Init.EOCSelection          = DISABLE;

    if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    {
        /* ADC initialization Error */
        adc_error_handler();
    }

    /*##-2- Configure ADC regular channel ######################################*/
    sConfig.Channel      = ADC_CHANNEL_8;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset       = 0;

    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        /* Channel Configuration Error */
        adc_error_handler();
    }

    // Configure the secondary channel

    sConfig.Channel      = ADC_CHANNEL_7;
    sConfig.Rank         = 2;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset       = 0;

    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        /* Channel Configuration Error */
        adc_error_handler();
    }


    /*##-3- Start the conversion process #######################################*/
    if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)adc_buffer, ADC_BUF_SIZE) != HAL_OK)
    {
        /* Start Conversation Error */
        adc_error_handler();
    }
}

volatile int prev_x = 0;

extern int pending_normalization;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) {
    for( int j = ADC_BUF_SIZE/4; j != ADC_BUF_SIZE/2; ++j ) {
        uint32_t x_val = adc_buffer[j*2], y_val = adc_buffer[j*2+1];
        x_val = (x_val * adc_spectrogram->npoints) / 4096;
        y_val = adc_spectrogram->size_y - (adc_spectrogram->size_y * y_val)/4096;
        if(x_val < 5) prev_x = 0;
        if(x_val > prev_x) {
            if(pending_normalization) {
                adc_spectrogram->data_normal[x_val] = adc_spectrogram->size_y/2 - y_val;
            }
            adc_spectrogram->data[x_val] = y_val + adc_spectrogram->data_normal[x_val];
            prev_x = x_val;
        }
    }
    if(pending_normalization) {
        --pending_normalization;
    }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle) {
    for( int j = 0; j != ADC_BUF_SIZE/4; ++j ) {
        uint32_t x_val = adc_buffer[j*2], y_val = adc_buffer[j*2+1];
        x_val = (x_val * adc_spectrogram->npoints) / 4096;
        y_val = adc_spectrogram->size_y - (adc_spectrogram->size_y * y_val)/4096;
        if(x_val < 5) prev_x = 0;
        if(x_val > prev_x) {
            if(pending_normalization) {
                adc_spectrogram->data_normal[x_val] = adc_spectrogram->size_y/2 - y_val;
            }
            adc_spectrogram->data[x_val] = y_val + adc_spectrogram->data_normal[x_val];
            prev_x = x_val;
        }
    }
}

/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  static DMA_HandleTypeDef  hdma_adc;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* ADC3 Periph clock enable */
  ADCx_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  ADCx_CHANNEL_GPIO_CLK_ENABLE();
  /* Enable DMA2 clock */
  DMAx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* ADC Channel GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA streams ##########################################*/
  /* Set the parameters to be configured */
  hdma_adc.Instance = ADCx_DMA_STREAM;

  hdma_adc.Init.Channel  = ADCx_DMA_CHANNEL;
  hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_adc.Init.Mode = DMA_CIRCULAR;
  hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;

  HAL_DMA_Init(&hdma_adc);

  /* Associate the initialized DMA handle to the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{

  /*##-1- Reset peripherals ##################################################*/
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC Channel GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
}
