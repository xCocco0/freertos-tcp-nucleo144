
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
DEBUG = 1
release: DEBUG=0

# optimization
ifeq ($(DEBUG), 1)
OPT = -Og
endif

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
CORE_DIR = Core
MIDDLEWARES_DIR = Middlewares
DRIVERS_DIR = Drivers

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
# C sources for Core (automatically selected)
C_SOURCES_CORE = $(wildcard $(CORE_DIR)/Src/*.c)
#Core/Src/main.c \
Core/Src/freertos.c \
Core/Src/stm32f4xx_it.c \
Core/Src/stm32f4xx_hal_msp.c \
Core/Src/stm32f4xx_hal_timebase_tim.c \
Core/Src/printf_stdarg.c

# C sources for Driver
C_SOURCES_DRIVERS = \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cec.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dfsdm.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dsi.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpi2c.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpi2c_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpsmbus.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpsmbus_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_irda.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_lptim.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_mmc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nand.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pccard.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_qspi.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smartcard.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smbus.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spdifrx.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_crc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dac.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma2d.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmpi2c.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_lptim.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_pwr.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rng.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c
#$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_alarm_template.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c \
$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_msp_template.c \

# C sources for Middleware
C_SOURCES_MIDDLEWARES = \
$(MIDDLEWARES_DIR)/FreeRTOS/croutine.c \
$(MIDDLEWARES_DIR)/FreeRTOS/event_groups.c \
$(MIDDLEWARES_DIR)/FreeRTOS/list.c \
$(MIDDLEWARES_DIR)/FreeRTOS/queue.c \
$(MIDDLEWARES_DIR)/FreeRTOS/stream_buffer.c \
$(MIDDLEWARES_DIR)/FreeRTOS/tasks.c \
$(MIDDLEWARES_DIR)/FreeRTOS/timers.c \
$(MIDDLEWARES_DIR)/FreeRTOS/CMSIS_RTOS/cmsis_os.c \
$(MIDDLEWARES_DIR)/FreeRTOS/portable/MemMang/heap_4.c \
$(MIDDLEWARES_DIR)/FreeRTOS/portable/GCC/ARM_CM4F/port.c \
\
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_ARP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_BitConfig.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_DHCP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_DHCPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_DNS.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_DNS_Cache.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_DNS_Callback.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_DNS_Networking.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_DNS_Parser.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_ICMP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IP_Timers.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IP_Utils.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IPv4_Sockets.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IPv4_Utils.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IPv6_Sockets.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_IPv6_Utils.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_ND.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_RA.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_Routing.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_Sockets.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_Stream_Buffer.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_IP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_IP_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_IP_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_Reception.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_State_Handling.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_State_Handling_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_State_Handling_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_Transmission.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_Transmission_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_Transmission_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_Utils.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_Utils_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_Utils_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_TCP_WIN.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_Tiny_TCP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_UDP_IP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_UDP_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/FreeRTOS_UDP_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/portable/BufferManagement/BufferAllocation_2.c

C_SOURCES = $(C_SOURCES_CORE) $(C_SOURCES_DRIVERS) $(C_SOURCES_MIDDLEWARES)

# ASM sources
ASM_SOURCES =  \
startup_stm32f429xx.s

# ASM sources
ASMM_SOURCES = 

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
C_INCLUDES_CORE = \
-I$(CORE_DIR)/Inc \
-I$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Inc \
-I$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Inc/Legacy \
-I$(DRIVERS_DIR)/CMSIS/Device/ST/STM32F4xx/Include \
-I$(DRIVERS_DIR)/CMSIS/Include \
-I$(MIDDLEWARES_DIR)/FreeRTOS/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS/portable/GCC/ARM_CM4F \
-I$(MIDDLEWARES_DIR)/FreeRTOS/CMSIS_RTOS \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/portable/Compiler/GCC

C_INCLUDES_DRIVERS = \
-I$(CORE_DIR)/Inc \
-I$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Inc \
-I$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Inc/Legacy \
-I$(DRIVERS_DIR)/CMSIS/Device/ST/STM32F4xx/Include \
-I$(DRIVERS_DIR)/CMSIS/Include

C_INCLUDES_MIDDLEWARE = \
-I$(CORE_DIR)/Inc \
-I$(DRIVERS_DIR)/CMSIS/Include \
-I$(MIDDLEWARES_DIR)/FreeRTOS/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS/CMSIS_RTOS \
-I$(MIDDLEWARES_DIR)/FreeRTOS/portable/GCC/ARM_CM4F \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/portable/Compiler/GCC \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/include
#-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/portable/NetworkInterface/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/portable/NetworkInterface/STM32Fxx

C_INCLUDES = $(C_INCLUDES_CORE) $(C_INCLUDES_DRIVERS) $(C_INCLUDES_MIDDLEWARE)

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
	$(call summary,"[HEX] \t\t\t $@")
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	$(call summary,"[BIN] \t\t\t $@")
	
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
