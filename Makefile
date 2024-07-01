
# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# Based on the Makefile automatically generated
# 	using CubeMX
# ------------------------------------------------

######################################
# target
######################################
TARGET = test
RMFLAGS = -r

######################################
# ctags variables
######################################

CTAGS = 1
CTAGS_PROG = ctags
CTAGS_OPT = --recurse=yes --exclude=build --tag-relative --extra=+f
CTAGS_TARGET = tags

######################################
# debug
######################################
# debug build?

ifneq ($(DEBUG), 1)
ifneq ($(DEBUG), 0)
DEBUG=1
endif
endif

# optimization
ifeq ($(DEBUG), 1)

OPT = -Og -O0
else
OPT = -O3
endif

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
CORE_DIR = Core
MIDDLEWARES_DIR = Middlewares
DRIVERS_DIR = Drivers
UTILITIES_DIR = Utilities

######################################
# verbose
######################################
ifndef ($(VERBOSE))
VERBOSE=0
endif

ifeq ($(VERBOSE),0)
define summary
	@echo "$(1)"
endef
endif

######################################
# source
######################################

# This will set C_SOURCES
include Sources.inc

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
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F429xx \
-DSTM32F4xx
ifeq ($(DEBUG), 1)
C_DEFS += -DDEBUG
endif

# AS includes
AS_INCLUDES =  \
-ICore/Inc

# C includes

# This will set C_INCLUDES
include Includes.inc

#######################################
# CFLAGS
#######################################

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
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
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin tags
	@echo Build target compiled successfully

#######################################
# build the application
#######################################
# list of objects
OBJECTS_CORE = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES_CORE:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES_CORE)))
OBJECTS_DRIVERS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES_DRIVERS:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES_DRIVERS)))
OBJECTS_MIDDLEWARES = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES_MIDDLEWARES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES_MIDDLEWARES)))
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_MIDDLEWARES)
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

ifeq ($(VERBOSE),0)
.SILENT: $(OBJECTS) $(addprefix $(BUILD_DIR)/$(TARGET),.elf .bin .hex)
endif

$(OBJECTS_CORE): $(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $(C_INCLUDES) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	$(call summary,"[Core] [CC] $@")

$(OBJECTS_DRIVERS): $(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $(C_INCLUDES) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	$(call summary,"[Drivers] [CC] $@")

$(OBJECTS_MIDDLEWARES): $(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $(C_INCLUDES) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	$(call summary,"[Middlewares] [CC] $@")

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
	$(call summary,"[AS] $@")

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
	$(call summary,"[AS] $@")

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(call summary,"[CC] $@")
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	$(call summary,"[HEX] $@")
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	$(call summary,"[BIN] $@")
	
$(BUILD_DIR):
	@mkdir $@		


#######################################
# install
#######################################
install: | $(BUILD_DIR)/$(TARGET).bin
	./setup flash $(BUILD_DIR)/$(TARGET).bin
	@echo Target flashed

#######################################
# clean up
#######################################
clean:
	-$(RM) $(RMFLAGS) $(BUILD_DIR)
	-$(RM) $(RMFLAGS) $(CTAGS_TARGET)
	@$(MAKE) -f $(UTILITIES_DIR)/Makefile clean
  
#######################################
# utilities
#######################################
utilities:
	@$(MAKE) -f $(UTILITIES_DIR)/Makefile
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

#######################################
# ctags
#######################################
ifeq ($(CTAGS), 1)
tags: | $(BUILD_DIR)/$(TARGET).elf
	$(CTAGS_PROG) $(CTAGS_OPT) -f ./$(CTAGS_TARGET) .
else
tags:
endif

#######################################
# .phony
#######################################
.phony: clean

# *** EOF ***
