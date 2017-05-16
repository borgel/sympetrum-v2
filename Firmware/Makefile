######################################
# Makefile by CubeMX2Makefile.py
######################################

######################################
# target
######################################
TARGET = sympetrum-v2
#CPU = STM32F051x8
CPU = STM32F030x6

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
ifeq ($(CPU), STM32F051x8)
   STARTUP_FILE = Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f051x8.s
   LDSCRIPT = SW4STM32/STM32F051R8Tx_FLASH.ld
else ifeq ($(CPU), STM32F030x6)
   STARTUP_FILE = Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030x6.s
   LDSCRIPT = SW4STM32/STM32F030K6Tx_FLASH.ld
endif

# may want to be more selective about this in the future
C_SOURCES  = $(wildcard Src/*.c)
C_SOURCES += $(wildcard Drivers/STM32F0xx_HAL_Driver/Src/*.c)

# FIXME this is a crappy hack, but wants to fight makefiles forever?
# add submodules
C_SOURCES += $(wildcard submodules/baf/src/*.c)
C_SOURCES += $(wildcard submodules/yabi/src/*.c)

ASM_SOURCES = $(STARTUP_FILE)

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
C_DEFS = -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -D$(CPU)
# includes for gcc
#FIXME find a better way of including all these header search paths
C_INCLUDES = -IInc/ -IDrivers/STM32F0xx_HAL_Driver/Inc/ -IDrivers/CMSIS/Device/ST/STM32F0xx/Include/ -IDrivers/CMSIS/Include -IDrivers/STM32F0xx_HAL_Driver/Inc/Legacy
C_INCLUDES += -Isubmodules/baf/include -Isubmodules/yabi/include
AS_INCLUDES = $(C_INCLUDES)

# compile gcc flags
ASFLAGS = -mthumb -mcpu=cortex-m0 $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = -mthumb -mcpu=cortex-m0 $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
# Generate dependency information
CFLAGS += -std=c99 -MD -MP -MF .dep/$(@F).d

#######################################
# LDFLAGS
#######################################
# link script set above based on CPU
# libraries
LIBS = -lc -lnosys -lm
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
