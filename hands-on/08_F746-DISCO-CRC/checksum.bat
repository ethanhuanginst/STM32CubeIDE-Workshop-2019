arm-none-eabi-objcopy  -O ihex  08_F746-DISCO-CRC.elf  "08_F746-DISCO-CRC.hex"
srec_cat 08_F746-DISCO-CRC.hex -Intel -fill 0xFF 0x08000000 0x08007FFC -STM32 0x08007FFC -o 08_F746-DISCO-CRC_SRECORD.hex -Intel
