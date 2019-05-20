C:\ST\STM32CubeIDE_1.0.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.7-2018-q2-update.win32_1.0.0.201904181610\tools\arm-none-eabi\bin\objcopy.exe  -O ihex  06_F746-DISCO-CRC.elf  "06_F746-DISCO-CRC.hex"
..\srec_cat.exe 06_F746-DISCO-CRC.hex -Intel -fill 0xFF 0x08000000 0x08007FFC -STM32 0x08007FFC -o 06_F746-DISCO-CRC_SRECORD.hex -Intel
