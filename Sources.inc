
# ------------------------------------------------
# Sources list for Makefile
#
# This will set the variable C_SOURCES listing all
# the files that will be compiled
# ------------------------------------------------

# C sources for Core (automatically selected)
C_SOURCES_CORE = $(wildcard $(CORE_DIR)/Src/*.c)

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
$(MIDDLEWARES_DIR)/FreeRTOS/portable/MemMang/heap_4.c \
$(MIDDLEWARES_DIR)/FreeRTOS/portable/GCC/ARM_CM4F/port.c \
\
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_ARP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_BitConfig.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_DHCP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_DHCPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS_Cache.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS_Callback.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS_Networking.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_DNS_Parser.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_ICMP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IP_Timers.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IP_Utils.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv4_Sockets.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv4_Utils.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv6_Sockets.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_IPv6_Utils.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_ND.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_RA.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_Routing.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_Sockets.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_Stream_Buffer.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_IP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_IP_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_IP_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Reception.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_State_Handling.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_State_Handling_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_State_Handling_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Transmission.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Transmission_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Transmission_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Utils.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Utils_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_Utils_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_TCP_WIN.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_Tiny_TCP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_UDP_IP.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_UDP_IPv4.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/FreeRTOS_UDP_IPv6.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/portable/BufferManagement/BufferAllocation_2.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/STM32Fxx/stm32fxx_hal_eth.c \
$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/Common/phyHandling.c \
\
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/portable/NetworkInterface/Common/NetworkWrapper.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_Ancillary.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_Controller.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_DS.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_NetworkSchedulerBlock.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_NetworkScheduler.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_NetworkSchedulerQueue.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_Sockets.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_Timebase.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_Timestamp.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/FreeRTOS_TSN_VLANTags.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/modules/BasicSchedulers/BasicSchedulers.c \
$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/modules/CreditBasedScheduler/SchedCBS.c

C_SOURCES = $(C_SOURCES_CORE) $(C_SOURCES_DRIVERS) $(C_SOURCES_MIDDLEWARES)

# ASM sources
ASM_SOURCES =  \
startup_stm32f429xx.s

# ASM sources
ASMM_SOURCES = 
