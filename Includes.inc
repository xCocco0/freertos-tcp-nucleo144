
# ------------------------------------------------
# Includes list for Makefile
#
# This will set the variable C_INCLUDES listing
# all the directories that will be add to the
# include list
# ------------------------------------------------

C_INCLUDES_CORE = \
-I$(CORE_DIR)/Inc \

C_INCLUDES_DRIVERS = \
-I$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Inc \
-I$(DRIVERS_DIR)/STM32F4xx_HAL_Driver/Inc/Legacy \
-I$(DRIVERS_DIR)/CMSIS/Include \
-I$(DRIVERS_DIR)/CMSIS/Device/ST/STM32F4xx/Include \
-I$(DRIVERS_DIR)/CMSIS/Device/ST/STM32F4xx/Include

C_INCLUDES_MIDDLEWARE = \
-I$(MIDDLEWARES_DIR)/FreeRTOS/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS/portable/GCC/ARM_CM4F \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/portable/NetworkInterface/STM32Fxx \
-I$(MIDDLEWARES_DIR)/FreeRTOS-Plus-TCP/source/portable/Compiler/GCC \
-I$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/portable/NetworkInterface/include \
-I$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/modules/BasicSchedulers \
-I$(MIDDLEWARES_DIR)/FreeRTOS-TSN-Compatibility-Layer/source/modules/CreditBasedScheduler

# Note: FreeRTOS-Plus-TCP/portable/NetworkInterface must be include before the drivers
C_INCLUDES = $(C_INCLUDES_MIDDLEWARE) $(C_INCLUDES_CORE) $(C_INCLUDES_DRIVERS)
