/**
  @page DSP DEMONSTRATION

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    STM32F746_DSPDEMO/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F746_DSPDEMO.
  ******************************************************************************
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
  @endverbatim

@par Application Description

This application describes how to use the STM32F746 with CMSIS library.

 At the beginning of the main program the HAL_Init() function is called to reset 
 all the peripherals, initialize the Flash interface and the systick.
 Then the SystemClock_Config() function is used to configure the system
 clock (SYSCLK) to run at 200 MHz. 

 After LCD and touch screen initialization, user need to calibrate the screen so the MCU recognise 
 the location where the user touches the screen, a menu is displayed on the screen 
 
 The menu contains a several choices such FFT or FIR example. 

For FFT there is some other choices: Q15, Q31, F32.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - X-CUBE-DSPDEMO/Src/main.c                 Main program
  - X-CUBE-DSPDEMO/calibration.c              Calibrate touch screen
  - X-CUBE-DSPDEMO/DISPLAY.c				  Configure the GUI
  - X-CUBE-DSPDEMO/fft_processing.c			  Configuration of FFT example 
  - X-CUBE-DSPDEMO/fir_processing.c			  Configuration of FIR example 
  - X-CUBE-DSPDEMO/global.c					  Configuration of the MCU peripherals
  - X-CUBE-DSPDEMO/rtc.c					  Configuration of the RTC to save the calibration TS values
  - X-CUBE-DSPDEMO/Src/system_stm32f7xx.c     STM32F7xx system clock configuration file
  - X-CUBE-DSPDEMO/Src/stm32f7xx_it.c         Interrupt handlers 
  - X-CUBE-DSPDEMO/Src/stm32f7xx_hal_msp.c    HAL MSP module
  - X-CUBE-DSPDEMO/Inc/main.h                 Main program header file  
  - X-CUBE-DSPDEMO/Inc/stm32f7xx_hal_conf.h   HAL Configuration file
  - X-CUBE-DSPDEMO/Inc/stm32f7xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This application runs on STM32F746xx Devices.
    
  - This application has been tested with STM32F746-Discovery RevB board and can be
    easily tailored to any other supported device and development board.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
