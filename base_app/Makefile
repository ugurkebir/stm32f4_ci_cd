# ==== Project ====
TARGET    = stm32f4_base_app
DEBUG     = 1
OPT       = -Og
BUILD_DIR = build

# ==== Sources ====
C_SOURCES = \
  src/main.c \
  src/stm32f4xx_it.c \
  src/stm32f4xx_hal_msp.c \
  src/system_stm32f4xx.c \
  syscalls.c \
  sysmem.c
  
# HAL sources (kullandıklarına göre genişlet)
HAL_SOURCES = \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
  Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c

C_SOURCES += $(HAL_SOURCES)




# GCC sürümü startup dosyası (gerekirse yolunu düzelt)
ASM_SOURCES = \
  Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s

# ==== Tools ====
PREFIX  = arm-none-eabi-
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

# ==== MCU flags (tek kaynaktan) ====
CPU       = -mcpu=cortex-m4
FPU       = -mfpu=fpv4-sp-d16
FLOAT_ABI = -mfloat-abi=hard
MCU       = $(CPU) -mthumb $(FPU) $(FLOAT_ABI)

# ==== Defines & Includes ====
C_DEFS = \
  -DUSE_HAL_DRIVER \
  -DSTM32F407xx

C_INCLUDES = \
  -IInc \
  -IDrivers/STM32F4xx_HAL_Driver/Inc \
  -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
  -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
  -IDrivers/CMSIS/Include

# ==== Flags ====
CFLAGS  = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -ffunction-sections -fdata-sections
CFLAGS += -MMD -MP -g -gdwarf-2
ASFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -ffunction-sections -fdata-sections
ifeq ($(DEBUG),0)
CFLAGS := $(filter-out -Og,$(CFLAGS))
CFLAGS += -O2
endif

# ==== Linker ====
LDSCRIPT = STM32F407VGTX_FLASH.ld
LDFLAGS  = $(MCU) -T$(LDSCRIPT) -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# Semihosting seçimi: 0=kapalı (syscalls.c kullan), 1=açık (rdimon, syscalls.c çıkar)
SEMIHOST ?= 0
ifeq ($(SEMIHOST),1)
  C_SOURCES := $(filter-out syscalls.c,$(C_SOURCES))
  LDFLAGS   += --specs=rdimon.specs
  LIBS      = -lc -lm -lrdimon
else
  LDFLAGS   += --specs=nosys.specs
  LIBS      = -lc -lm -lnosys
endif

# ==== Objects ====
OBJECTS  = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# ==== Rules ====
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	-rm -rf $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)

# ==== Extras ====
flash: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $< verify reset exit"

flash-stlink: $(BUILD_DIR)/$(TARGET).hex
	st-flash --format ihex write $<

debug: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg &
	$(PREFIX)gdb -ex "target remote localhost:3333" -ex "monitor reset halt" -ex "load" $<

size: $(BUILD_DIR)/$(TARGET).elf
	$(SZ) --format=berkeley $<

size-detailed: $(BUILD_DIR)/$(TARGET).elf
	$(SZ) -A -x $<
