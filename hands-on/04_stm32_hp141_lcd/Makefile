EXECUTABLE=out.elf
STM32_LIBS=STM32Cube_FW_F7_V1.5.0

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld 
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump

DEFS = -DSTM32F746xx -DUSE_HAL_DRIVER -DUSE_STM32746G_DISCOVERY
MCU = cortex-m7
MCFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard
STD = -std=gnu11

STM32_INCLUDES = -I$(STM32_LIBS)/Utilities \
	-I$(STM32_LIBS)/Utilities/Fonts \
	-I$(STM32_LIBS)/Drivers/BSP/STM32746G-Discovery \
	-I$(STM32_LIBS)/Drivers/BSP/Components/Common \
	-I$(STM32_LIBS)/Drivers/CMSIS/Include/ \
	-I$(STM32_LIBS)/Drivers/CMSIS/Device/ST/STM32F7xx/Include \
    -I$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Inc \
    -I$(STM32_LIBS)/Middlewares/ST/STemWin/inc \

OPTIMIZE = -O0

CFLAGS	= $(MCFLAGS) $(STD) -g $(OPTIMIZE)  $(DEFS) -Iinc -I./ -I./ $(STM32_INCLUDES) -L$(STM32_LIBS)/Middlewares/ST/STemWin/Lib  -Wl,-T,src/sys/STM32F746NGHx_FLASH.ld

AFLAGS	= $(MCFLAGS)

SRC = \
	src/main.c \
	src/adc.c \
	src/maintask.c \
	src/options.c \
	src/spectrogram.c \
	src/set_frequencyDLG.c \
	src/menuDLG.c \
	src/sys/system_stm32f7xx.c \
	src/sys/stm32f7xx_it.c \
	src/sys/syscalls.c \
	src/sys/GUIConf.c \
	src/sys/LCDConf.c \
	src/sys/startup_stm32f746xx.s \
	$(STM32_LIBS)/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c \
	$(STM32_LIBS)/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c \
	$(STM32_LIBS)/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.c \
	$(STM32_LIBS)/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c \
	$(STM32_LIBS)/Drivers/BSP/Components/ft5336/ft5336.c \
	$(STM32_LIBS)/Middlewares/ST/STemWin/OS/GUI_X.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c \
	$(STM32_LIBS)/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \


OBJDIR = .
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ -l:STemWin532_CM7_GCC_ot.a -lm

debug_server:
	sudo openocd -f board/stm32f7discovery.cfg

debug_gdb:
	arm-none-eabi-gdb out.elf -ex "target remote localhost:3333" -ex "monitor reset halt" -ex "load" -ex "monitor reset halt" -ex "b main" -ex "c"
