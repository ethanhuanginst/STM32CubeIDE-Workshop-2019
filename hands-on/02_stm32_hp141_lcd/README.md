# Hands-on: stm32_hp141_lcd

## Introduction

This hands-on is originally from the Github repository: **[stm32_hp141_lcd](https://github.com/schnommus/stm32_hp141_lcd)**. It is designed to run on an STM32F746G Discovery board used to upgrade the CRT of an HP141T spectrum analyzer. [Youtube is available](https://youtu.be/CwwRvqHGyts). 

This hands-on is cloned from this interesting project and STM32Cube_FW_F7_V1.5.0 folder with only necessary files is put into this hands-on folder. The project could be compiled without error and generates **<u>out.elf</u>** as shown below:

![](../../docs/imgs/hands-on/stm32_hp141_lcd-make-all.png)

Please use STM32CubeProgrammer to flash out.elf into STM32F746-DISCO:

![](../../docs/imgs/hands-on/stm32_hp141_lcd-stm32cubeprogrammer.png)

After programming out.elf, your board should have the following result:

![](../../docs/imgs/hands-on/stm32f746-disco-hp141.jpg)

Now, let's see how to use STM32CubeIDE to build and debug such Makefile-base project.



## Steps

1. Start a new STM32 project
   
   * Select "Help -->Information Center":
     ![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-0.png)
   
     
   
   * Press "Start new STM32 project":
     ![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-1.png)
   * Select "32F746DISCOVERY" board from "Board Selector" and press "Next" button:
     ![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-2.png)
   * Assign "Project Name" and "Location", and make sure "Targeted Project Type" is "Empty". Press "Finish" button when all is set.
     ![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-3.png)
   
5. Prepare for compiling   
   * Delete all folders and files:
![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-5.png)
   * Drag all "stm32_hp141_lcd" file (except out.elf) and folders into "stm32_hp141_lcd" in Project Exploere of STM32CubeIDE:
     ![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-6.png)
   * Select "Copy files and folders" and then press "OK":
![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-7.png)
   
8. Compile the code
   
   * Select "Properties":
![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-9.png)
   
   * Modify "Builder Settings" (uncheck "Generate Makefiles automatically" and delete string "Debug" in Build directory) as shown below :
![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-10.png)
     
   * Select "Build Project":
![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-8.png)
     
     

11. Start debug

    * Select "Debug As --> STM32 MCU C/C++ Application"
    ![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-11.png)
    * Press "OK" button in "Edit Configuration":
    ![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-13.png)
    * Press "Resume" button to run the code:
    ![](../../docs/imgs/hands-on/stm32_hp141_lcd-start-new-project-12.png)

