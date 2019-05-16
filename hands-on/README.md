# Hands-on
**Under construction...**

All reference projects will be pushed to the repository by 09:30AM on 2019.05.22 (WED).

The following topics are planned to be covered by several hands-on:

* Start/Import STM32 Project
* MX Perspective (IOC Editor)
  * Pinout&Configuration
  * Clock Configuration
  * Project Manager
* Build Configuration  	
* Debug Configuration
  * Debugger
  * External Loader
* Debug Perspective
  * SFR View (Peripheral Registers)
  * Register View (Core Registers)
  * Live Expression 
  * HardFault Analysis
  * SWV Data timeline
  * Dynamic printf
  
* IO redirection
* Add static library
* Post-build  
* Build Analyzer, Linker Script and Startup Code



# Hands-on: X-CUBE-DSPDEMO

1. Build

2. Run/Debug to see how this demo works.

3. Add the code below in fir_processing.c

   ```C
#ifdef TEST_SWV
   float32_t swv_aFIR_F32_1kHz_15kHz = 0.0f;
   float32_t swv_aFIR_F32_Output = 0.0f;
   #endif```
   ```
   
4. Modify the code below (enclosed by #ifdef TEST_SWV, #else and #endif) inside FIR_PROCESSING_F32Process() definition in fir_processing.c under Src\:
   ```C
void FIR_PROCESSING_F32Process(void)
{ 
     ...
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
   ```

5. Define TEST_SWV in as shown below and then build project:

   ![](../docs/imgs/hands-on/x-cube-dspdemo_define_TEST_SWV.png)
   
6. Setup "Debug Configuration"

   * Right click on project, select "Debug As --> Debug Configuration" as shown below
     
     ![](../docs/imgs/hands-on/x-cube-dspdemo_debug_configurations.png)
     
   * Go to "Debugger" tab, check "Enable" and then set Core Clock 200MHz as shown below:
     
     ![](../docs/imgs/hands-on/x-cube-dspdemo_debug_configurations_swv.png)
     

   

7. Run Debug again and do not press "Resume" yet.

8. Setup SWV Data Trace Timeline Graph

   * Select "Window --> Show View --> SWV --> SWV Data Trace Timeline Graph" to Enable SWV Data Trace Timeline Graph view as shown below

     ![](../docs/imgs/hands-on/x-cube-dspdemo_swv_data_trace_timeline_graph.png)

   * Press "Configure Trace " button as shown below:
     ![](../docs/imgs/hands-on/x-cube-dspdemo_swv_data_trace_timeline_graph_config.png)

   * Add "swv_aFIR_F32_1kHz_15kHz" and "swv_aFIR_F32_Output" in Var/Addr of Comparator 0 and 1 as shown below:
     ![](../docs/imgs/hands-on/x-cube-dspdemo_swv_data_trace_timeline_graph_add_var.png)

   * Press "Start Trace" button as shown below:
     ![](../docs/imgs/hands-on/x-cube-dspdemo_swv_data_trace_timeline_graph_start-trace.png)

9. Press "Resume" button and you will see waveform in SWV Data Trace Timeline Graph aligned with that in LCD:
   ![](../docs/imgs/hands-on/x-cube-dspdemo_swv_data_trace_timeline_graph_resume.png)
   

   ![](../docs/imgs/hands-on/x-cube-dspdemo_swv_data_trace_timeline_graph_waveform.png)







# Hands-on: stm32_hp141_lcd

This hands-on is originally from the Github repository: **[stm32_hp141_lcd](https://github.com/schnommus/stm32_hp141_lcd)**. It is designed to run on an STM32F746G Discovery board used to upgrade the CRT of an HP141T spectrum analyzer. [Youtube is available](https://youtu.be/CwwRvqHGyts). 

This hands-on is cloned from this interesting project and STM32Cube_FW_F7_V1.5.0 folder with only necessary files is put into this hands-on folder. The project could be compiled without error and generates **out.elf** as shown below:

![](../docs/imgs/hands-on/stm32_hp141_lcd-make-all.png)

Please use STM32CubeProgrammer to flash out.elf into STM32F746-DISCO to play with it:

![](../docs/imgs/hands-on/stm32_hp141_lcd-stm32cubeprogrammer.png)

After programming out.elf, your board should have the following result:

![](../docs/imgs/hands-on/stm32f746-disco-hp141.jpg)

Now, let's see how to use STM32CubeIDE to build and debug such makefile-base project.



# Hands-on: QSPI_PreInitConfig

Purpose:



Procedure:

1. Point out project folder

2. Copy SW4STM32 folder and then change the name from <u>STM32746G-DISCOVERY</u> to <u>STM32746G-DISCOVERY\_QSPI_PreInitConfig</u> in .project file.

3. Import

4. Build

5. Debug

6. Watch LED blinking
   
   ![](../docs/imgs/hands-on/QSPI_PreInitConfig_LED_blinking.jpg)
   
7. Set breakpoint at BSP_LED_Toggle inside GpioToggle() and watch the code really stops here which is located in QSPI.
   
   ![](../docs/imgs/hands-on/QSPI_PreInitConfig_breakpoint.png)
   
   ```
   
   ```

  ```

  ```