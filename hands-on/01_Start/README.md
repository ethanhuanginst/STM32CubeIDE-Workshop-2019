# Hands-on: Start New STM32 Project

1. After running STM32CubeIDE, STM32CubeIDE Launcher appears as shown below:
   ![](../../docs/imgs/hands-on/01-000.png)

   Please select a location to store the workspace or simply use the default workspace for convenient. After pressing "Launch" button, the following will appear:
![](../../docs/imgs/hands-on/01-001.png)
   
   Suggest to select "NO" for the following override setting for file association for **.cproject** file if you have some other Eclipse-base IDE installed in your system:  
   ![](../../docs/imgs/hands-on/01-002.png)
   
   Suggest to select "NO" for the following override setting for file association for **.project** file if you have some other Eclipse-base IDE installed in your system:  
   ![](../../docs/imgs/hands-on/01-003.png)
   
   
   
2. After getting into "Information Center", please press "Start new STM32 project" button on the left:
   ![](../../docs/imgs/hands-on/01-004.png)
   The following will pop up and may take some time:
   ![](../../docs/imgs/hands-on/01-005.png)
   
   
   
3. Select "Board Selector" and type "34F746" in "Part Number Search" to quickly sort out the board we want. Click 32F746DISCOVERY in Board List and then press "Next" button:  
   ![](../../docs/imgs/hands-on/01-006.png)
   Assign Project Name and suggest to keep "Use default location" checked, then press "Finish" button:  
   ![](../../docs/imgs/hands-on/01-007.png)

   Press "NO" for the following dialog:  
![](../../docs/imgs/hands-on/01-008.png)

   Press "YES" for the following dialog and suggest to check "Remember my decision":  
![](../../docs/imgs/hands-on/01-009.png)

   The initialization may take some time:  
![](../../docs/imgs/hands-on/01-010.png)

   The Project Creation may also take some time:  
![](../../docs/imgs/hands-on/01-011.png)
   
   
   
4. In IOC editor, please search for PI1 pin and set it to GPIO_Output
   ![](../../docs/imgs/hands-on/01-013_PI1.png)
   
   
   
5. Add the following code to toggle LED LD1 (controlled by PI1) inside USER CODE Block 3 inside main():
   
   ```c
          /* USER CODE BEGIN 3 */
          HAL_Delay(500);
          HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
        }
        /* USER CODE END 3 */
   ```
   
   
   
6. Press the following button to generate code:  
   ![](../../docs/imgs/hands-on/01-014_GenerateCode.png)
   
   
   
7. Right click on project icon and then select "Build Project":  
   ![](../../docs/imgs/hands-on/01-020_BuildProject.png)
   
   
   
8. Start to run debug

   * Right click on project icon and then select "Debug As --> 2 STM32 MCU C/C++ Application":
     ![](../../docs/imgs/hands-on/01-030_DebugAs.png)

   * Press "OK" in this dialog:

     ![](../../docs/imgs/hands-on/01-031.png)

   * Press "Switch" button and suggest to check "Remember my decision":  
     ![](../../docs/imgs/hands-on/01-032_SwitchToDebugPerspective.png)
     
   * The following error may appear when ST-LINK FW of your board is not new enough.  
     ![](../../docs/imgs/hands-on/01-033_st-link-error.png)
     
      1. If the error occurs, please press "OK" button in the picture above, and then press "OK" button in the picture below:  
     ![](../../docs/imgs/hands-on/01-034_st-link-error.png)
       2. Press "Open in update mode" button in the picture below:  
![](../../docs/imgs/hands-on/01-035_st-link-upgrade.png)
       3. Press "Update" button in the picture below:  
![](../../docs/imgs/hands-on/01-036_st-link-upgrade.png)
       4. ST-LINK upgrade starts after pressing "Update" button:  
![](../../docs/imgs/hands-on/01-037_st-link-upgrade.png)
       5. ST-LINK upgrade completes.  
![](../../docs/imgs/hands-on/01-038_st-link-upgrade.png)
     
     
   
9. After switching to debug perspective, press "Resume" button to run your code:  
   ![](../../docs/imgs/hands-on/01-041_RunDebug.png)



## Some More Features Worth Trying

1. MCU Post build outputs: HEX/BIN
2. Editor: Auto-complete
3. Editor: Macro expansion
4. Editor: Code navigation

