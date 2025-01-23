######################################
# target
######################################
TARGET = stm32f429i_disk1_example
FW_START_ADDRESS = 0x08000000

STM32DRV_DIR = ./ext/lib-stm32drv
SCRIPTS_PATH = $(STM32DRV_DIR)/tools
CMSIS_INCLUDE_DIR = $(STM32DRV_DIR)
STM32DRV_INCLUDE_DIR = $(STM32DRV_DIR)/include
STM32DRV_SRC_DIR = $(STM32DRV_DIR)/src


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################

# C sources
C_SOURCES =  \
src/main.c \
src/bsp/leds.c \
src/bsp/bsp.c \
src/bsp/usart1.c \
src/bsp/spi5.c \
src/system/system.c \
src/system/exchange.c \
src/system/spi_control.c \
src/proto/proto.c \
src/proto/crc16.c \
src/proto/ringbuffer.c \
src/proto/parser.c \
$(STM32DRV_SRC_DIR)/rcc/rcc_f429xx.c \
$(STM32DRV_SRC_DIR)/uart/uart_f4xx.c \
$(STM32DRV_SRC_DIR)/spi/spi_f4xx.c \
$(STM32DRV_SRC_DIR)/systick/systick_cm4.c \
$(STM32DRV_SRC_DIR)/gpio/gpio_f4xx.c \

# ASM sources
ASM_SOURCES =  \
src/startup.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS = \
-DSTM32F429xx

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES = \
-I./include \
-I$(STM32DRV_DIR)/drv \
-I$(CMSIS_INCLUDE_DIR) \
-I$(STM32DRV_INCLUDE_DIR)


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2 -O0
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F429ZITx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin genver


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
	mkdir $@	

#######################################
# generate version
#######################################	
genver: $(BUILD_DIR)/$(TARGET).bin
	python3 $(SCRIPTS_PATH)/postimg.py $(BUILD_DIR)/$(TARGET).bin	

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

#######################################
# prog
#######################################

prog: $(BUILD_DIR)/$(TARGET).elf
	openocd -f board/stm32f429disc1.cfg -c "program build/$(TARGET).bin exit reset $(FW_START_ADDRESS)"

# *** EOF ***