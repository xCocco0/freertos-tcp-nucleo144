
#ifndef FREERTOS_IP_CONFIG_H
#define FREERTOS_IP_CONFIG_H

#include <stdint.h>

#define ipconfigBYTE_ORDER pdFREERTOS_LITTLE_ENDIAN

// suggested configuration from the driver readme
#define ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES   1
#define ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM        1
#define ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM        1
#define ipconfigZERO_COPY_RX_DRIVER                   1
#define ipconfigZERO_COPY_TX_DRIVER                   1
#define ipconfigUSE_LINKED_RX_MESSAGES                1
#define ipconfigSUPPORT_NETWORK_DOWN_EVENT            1

#define ipconfigUSE_DHCP_HOOK ipconfigDISABLE
#define ipconfigUSE_DHCP ipconfigDISABLE
#define ipconfigIPv4_BACKWARD_COMPATIBLE ipconfigENABLE

#define ipconfigUSE_RMII ipconfigENABLE

#define ipconfigREPLY_TO_INCOMING_PINGS ipconfigENABLE

#define ipconfigUSE_TCP ipconfigENABLE
#define ipconfigUSE_TCP_WIN ipconfigENABLE

#ifdef DEBUG
#define ipconfigHAS_DEBUG_PRINTF ipconfigENABLE
#define FreeRTOS_debug_printf( MSG )    do { configPRINTF( MSG ); configPRINTF( ("\r") ); } while( ipFALSE_BOOL )
#include "IPTraceMacro.h"
#endif

#endif /* FREERTOS_IP_CONFIG_H */
