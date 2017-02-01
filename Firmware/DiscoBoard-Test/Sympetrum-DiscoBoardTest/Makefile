######################################
# Makefile by CubeMX2Makefile.py
######################################

######################################
# target
######################################
TARGET = Sympetrum-DiscoBoardTest

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O0

#######################################
# pathes
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################

# may want to be more selective about this in the future
C_SOURCES  = $(wildcard Src/*.c)
C_SOURCES += $(wildcard Drivers/STM32F0xx_HAL_Driver/Src/*.c)

ASM_SOURCES = SW4STM32/startup_stm32f051x8.s

#######################################
# binaries
#######################################
CC = /usr/local/gcc-arm-none-eabi-6_2-2016q4/bin/arm-none-eabi-gcc
AS = /usr/local/gcc-arm-none-eabi-6_2-2016q4/bin/arm-none-eabi-gcc -x assembler-with-cpp
CP = /usr/local/gcc-arm-none-eabi-6_2-2016q4/bin/arm-none-eabi-objcopy
AR = /usr/local/gcc-arm-none-eabi-6_2-2016q4/bin/arm-none-eabi-ar
SZ = /usr/local/gcc-arm-none-eabi-6_2-2016q4/bin/arm-none-eabi-size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# macros for gcc
AS_DEFS =
C_DEFS = -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F051x8 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F051x8
# includes for gcc
AS_INCLUDES =
C_INCLUDES =
# compile gcc flags
# FIXME rm INC search paths?
ASFLAGS = -mthumb -mcpu=cortex-m0 $(AS_DEFS) -IInc/ -IDrivers/STM32F0xx_HAL_Driver/Inc/ -IDrivers/CMSIS/Device/ST/STM32F0xx/Include/ -IDrivers/CMSIS/Include -IDrivers/STM32F0xx_HAL_Driver/Inc/Legacy $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
#FIXME find a better way of including all these header search paths
CFLAGS = -mthumb -mcpu=cortex-m0 $(C_DEFS) -IInc/ -IDrivers/STM32F0xx_HAL_Driver/Inc/ -IDrivers/CMSIS/Device/ST/STM32F0xx/Include/ -IDrivers/CMSIS/Include -IDrivers/STM32F0xx_HAL_Driver/Inc/Legacy $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
# Generate dependency information
CFLAGS += -std=c99 -MD -MP -MF .dep/$(@F).d

#######################################
# LDFLAGS
#######################################
# link script
#LDSCRIPT = STM32F051R8Tx_FLASH.ld
LDSCRIPT = SW4STM32/Sympetrum-DiscoBoardTest/STM32F051R8Tx_FLASH.ld
# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = -mthumb -mcpu=cortex-m0 -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

flash:$(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x8000000
#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir -p $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY: clean all flash

# *** EOF ***
