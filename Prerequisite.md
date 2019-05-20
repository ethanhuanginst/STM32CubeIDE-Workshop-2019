# Prerequisite
1. Download and install STM32CubeIDE from the following link:

   https://www.st.com/en/development-tools/stm32cubeide.html

   ![](docs/imgs/Download_STM32CubeIDE.png)
   
   * Please note that user login is required to download STM32 software as shown below. Please register first if you do not have the account.
    ![](docs/imgs/ST_Registration.png)
     
   * As described in [STM32CubeIDE installation guide UM2563](https://www.st.com/resource/en/user_manual/dm00603964.pdf), **only 64-bit OS** versions are supported.

2. Download and install STM32Programmer from the following link:

   https://www.st.com/en/development-tools/stm32cubeprog.html

   ![](docs/imgs/Download_STM32CubeProgrammer.png)

   * Login/register is required.

3. Download STM32CubeF7 MCU package from the following link:

   https://www.st.com/en/embedded-software/stm32cubef7.html

   ![](docs/imgs/Download_STM32CubeF7.png)

   * Login/register is required.

4. Setting up network connection for STM32CubeIDE is necessary if your laptop is managed by the proxy of your company. 

   * Open STM32CubeIDE
   
   * Use default workspace
   
     ![](/docs/imgs/STM32CubeIDE_Launcher.png)
   
   * Suggest to choose "NO" for the following dialogs:

     .cproject file extension:
     
     ![](/docs/imgs/STM32CubeIDE_Override_File_Extension.png)
     
     .project file extension:
     
     ![](/docs/imgs/STM32CubeIDE_Override_File_Extension_2.png)
   
   * Window --> Preferences (as shown below)
     ![](docs/imgs/STM32CubeIDE_Proxy_1.png)
   * General --> Network Connection (as shown below)
     ![](docs/imgs/STM32CubeIDE_Proxy_2.png)
   
   * Check network connection by clicking "Check Connection" button in STM32Cube --> Firmware Updater (as shown below)
   
     ![](docs/imgs/STM32CubeIDE_Proxy_3.png)



5. To read README.md offline downloaded from Github, please download and install any markdown viewer/editor such as [Typora](https://typora.io/).