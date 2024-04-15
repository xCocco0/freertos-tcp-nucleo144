
#ifndef FREERTOS_IP_CONFIG_H
#define FREERTOS_IP_CONFIG_H

#include <stdint.h>

#define ipconfigBYTE_ORDER pdFREERTOS_LITTLE_ENDIAN

#define ipconfigUSE_DHCP_HOOK ipconfigDISABLE
#define ipconfigUSE_DHCP ipconfigDISABLE
#define ipconfigIPv4_BACKWARD_COMPATIBLE ipconfigENABLE

#define ipconfigUSE_RMII ipconfigENABLE

#ifdef DEBUG
#define ipconfigHAS_DEBUG_PRINTF ipconfigENABLE
#include "IPTraceMacro.h"
#endif

#endif /* FREERTOS_IP_CONFIG_H */
