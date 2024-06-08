
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "peripherals.h"
#include "FreeRTOS_ARP.h"

#include "FreeRTOS_TSN_NetworkScheduler.h"
#include "FreeRTOS_TSN_Controller.h"
#include "FreeRTOS_TSN_VLANTags.h"
#include "FreeRTOS_TSN_Sockets.h"
#include "FreeRTOS_TSN_DS.h"

#ifdef DEBUG
#include "logger.h"
#endif

#define DST_ADDR_4 "169.254.151.41"
#define DST_ADDR_6 "fe80::4535:c3a4:9457:6e5a"
#define DST_PORT 10000

#define SRC_ADDR_4 "169.254.151.42"
#define SRC_ADDR_6 "fe80::dead:beef"
#define SRC_PORT 10000


void vTaskTSNTest(void *argvument);
void vTaskUDPSendIPv4(void *argument);
void vTaskUDPSendIPv6(void *argument);
void vTaskTCPSendIPv4(void *argument);
void vTaskTCPSendIPv6(void *argument);

/**
 * @brief  Test
 * @param  argument: Not used
 * @retval None
 */
void vTaskTSNTest(void *argvument)
{
		BaseType_t xRet;


		/* -- create socket -- */
		configPRINTF( ("Creating socket...\r\n") );
		TSNSocket_t xSock = FreeRTOS_TSN_socket(FREERTOS_AF_INET4,
				 FREERTOS_SOCK_DGRAM, 
				 FREERTOS_IPPROTO_UDP);
		if( xSock == FREERTOS_TSN_INVALID_SOCKET ) {
			configPRINTF( ("Socket error\r\n") );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		if( FreeRTOS_TSN_setsockopt( xSock, 0, FREERTOS_SO_VLAN_CTAG_PCP, ( void * ) vlantagCLASS_1, sizeof( void * ) ) != 0 )
		{
			configPRINTF( ("setsockopt error\r\n") );
			for(;;);
		}
		if( FreeRTOS_TSN_setsockopt( xSock, 0, FREERTOS_SO_VLAN_STAG_PCP, ( void * ) vlantagCLASS_2, sizeof( void * ) ) != 0 )
		{
			configPRINTF( ("setsockopt error\r\n") );
			for(;;);
		}
		if( FreeRTOS_TSN_setsockopt( xSock, 0, FREERTOS_SO_DS_CLASS, ( void * ) diffservCLASS_AFxy(2,3), sizeof( void * ) ) != 0 )
		{
			configPRINTF( ("setsockopt error\r\n") );
			for(;;);
		}

		UBaseType_t xSourceAddrLen;
		struct freertos_sockaddr xDestinationAddress, xSourceAddress;
		xDestinationAddress.sin_addr = FreeRTOS_inet_addr(DST_ADDR_4);
		xDestinationAddress.sin_port = FreeRTOS_htons(DST_PORT);
		xDestinationAddress.sin_family = FREERTOS_AF_INET;
		xSourceAddress.sin_addr = FreeRTOS_inet_addr(SRC_ADDR_4);
		xSourceAddress.sin_port = FreeRTOS_htons(SRC_PORT);
		xSourceAddress.sin_family = FREERTOS_AF_INET;

		/* -- bind socket -- */
		configPRINTF( ("Binding socket...\r\n") );
		xRet = FreeRTOS_TSN_bind(xSock, &xSourceAddress, sizeof( xSourceAddress ) );
		if(xRet) {
			configPRINTF( ("Socket could not be bound: error %d\r\n", xRet) );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		MACAddress_t xDestMAC = { .ucBytes = {0x8C,0xDC,0xD4,0x90,0x5D,0x33} };
		NetworkEndPoint_t * pxEndPoint = FreeRTOS_FindEndPointOnNetMask( xDestinationAddress.sin_address.ulIP_IPv4, 14 );

		vTaskDelay(1000U);
		vARPRefreshCacheEntry( &xDestMAC, xDestinationAddress.sin_address.ulIP_IPv4, pxEndPoint);

		char cMsg[32];

		for(int i = 0;; ++i)
		{
			sprintf(cMsg, "Hello world n.%d", i);
			configPRINTF( ("Sending message \"%s\"...", cMsg) );

			xRet = FreeRTOS_TSN_sendto( xSock, cMsg, configMIN(sizeof(cMsg), strlen(cMsg)), 0,
					&xDestinationAddress, sizeof(xDestinationAddress) );
			configPRINTF( ("sent %d bytes.\r\n", xRet) );
			if(xRet < 0)
			{
				configPRINTF( ("sendto error %d.\r\n", xRet) );
				for(;;);
			}

			configPRINTF( ("Receiving...\r\n") );
			xRet = FreeRTOS_TSN_recvfrom( xSock, cMsg, sizeof( cMsg ), 0, &xSourceAddress, &xSourceAddrLen );
			if(xRet < 0)
			{
				configPRINTF( ("recvfrom error %d.\r\n", xRet) );
				for(;;);
			}
			configPRINTF( ("Received message \"%s\"\r\n", cMsg) );
			
			vTaskDelay( pdMS_TO_TICKS( 2000 ) );
		}
}


/**
 * @brief  Periodically send UDP packet using IPv4
 * @param  argument: Not used
 * @retval None
 */
void vTaskUDPSendIPv4(void *argument)
{
	BaseType_t xRet;

	configPRINTF( ("vTaskUDPSendIPv4 started\r\n") );
	for(;;) {
		
		/* -- create socket -- */
		configPRINTF( ("Creating socket...\r\n") );
		Socket_t xSock = FreeRTOS_socket(FREERTOS_AF_INET4,
				 FREERTOS_SOCK_DGRAM, 
				 FREERTOS_IPPROTO_UDP);
		if(xSock == FREERTOS_INVALID_SOCKET) {
			configPRINTF( ("Socket error\r\n") );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		/* -- bind socket -- */
		socklen_t xSize = sizeof(struct freertos_sockaddr);
		configPRINTF( ("Binding socket...\r\n") );
		xRet = FreeRTOS_bind(xSock, NULL, xSize);
		if(xRet) {
			configPRINTF( ("Socket could not be bound: error %d\r\n", xRet) );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		struct freertos_sockaddr xDestinationAddress;
		xDestinationAddress.sin_addr = FreeRTOS_inet_addr(DST_ADDR_4);
		xDestinationAddress.sin_port = FreeRTOS_htons(DST_PORT);
		xDestinationAddress.sin_family = FREERTOS_AF_INET;

		char cMsg[32];

		for(int i = 0; ; ++i) {

			sprintf(cMsg, "Hello world n.%d", i);
			configPRINTF( ("Sending message \"%s\"...", cMsg) );

			xRet = FreeRTOS_sendto( xSock, cMsg, configMIN(sizeof(cMsg), strlen(cMsg)), 0,
					&xDestinationAddress, sizeof(xDestinationAddress) );
			configPRINTF( ("sent %d bytes.\r\n", xRet) );
			configASSERT(xRet >= 0);
			HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
			vTaskDelay(5000UL);
			//vTaskDelay(1000UL / portTICK_PERIOD_MS);
		}
	}
}

/**
 * @brief  Periodically send UDP packet using IPv6
 * @param  argument: Not used
 * @retval None
 */
void vTaskUDPSendIPv6(void *argument)
{
	BaseType_t xRet;

	configPRINTF( ("vTaskUDPSendIPv6 started\r\n") );
	for(;;) {
		
		/* -- create socket -- */
		configPRINTF( ("Creating socket...\r\n") );
		Socket_t xSock = FreeRTOS_socket(FREERTOS_AF_INET6,
				 FREERTOS_SOCK_DGRAM, 
				 FREERTOS_IPPROTO_UDP);
		if(xSock == FREERTOS_INVALID_SOCKET) {
			configPRINTF( ("Socket error\r\n") );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		/* -- bind socket -- */
		socklen_t xSize = sizeof(struct freertos_sockaddr);
		configPRINTF( ("Binding socket...\r\n") );
		xRet = FreeRTOS_bind(xSock, NULL, xSize);
		if(xRet) {
			configPRINTF( ("Socket could not be bound: error %d\r\n", xRet) );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		struct freertos_sockaddr xDestinationAddress;
		FreeRTOS_inet_pton6( DST_ADDR_6, &xDestinationAddress.sin_address.xIP_IPv6.ucBytes);
		xDestinationAddress.sin_port = FreeRTOS_htons(DST_PORT);
		//xDestinationAddress.sin_family = FREERTOS_EF_INET6;

		char cMsg[32];

		for(int i = 0; ; ++i) {

			sprintf(cMsg, "Hello world n.%d", i);
			configPRINTF( ("Sending message \"%s\"...", cMsg) );

			xRet = FreeRTOS_sendto( xSock, cMsg, configMIN(sizeof(cMsg), strlen(cMsg)), 0,
					&xDestinationAddress, sizeof(xDestinationAddress) );
			configPRINTF( ("sent %d bytes.\r\n", xRet) );
			configASSERT(xRet >= 0);
			HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
			vTaskDelay(5000UL);
			//vTaskDelay(1000UL / portTICK_PERIOD_MS);
		}
	}
}


/**
 * @brief  Periodically send TCP packets using IPv4
 * @param  argument: Not used
 * @retval None
 */
void vTaskTCPSendIPv4(void *argument)
{
	BaseType_t xRet;

	configPRINTF( ("vTaskTCPSendIPv4 started\r\n") );
	for(;;) {

		/* -- create socket -- */
		configPRINTF( ("Creating socket...\r\n") );
		Socket_t xSock = FreeRTOS_socket(FREERTOS_AF_INET4,
				FREERTOS_SOCK_STREAM, 
				FREERTOS_IPPROTO_TCP);
		if(xSock == FREERTOS_INVALID_SOCKET) {
			HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
			configPRINTF( ("Socket error\r\n") );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		/* -- bind socket -- */
		struct freertos_sockaddr xSourceAddress;
		xSourceAddress.sin_addr = FreeRTOS_inet_addr(SRC_ADDR_4);
		xSourceAddress.sin_port = FreeRTOS_htons(SRC_PORT);
		xSourceAddress.sin_family = FREERTOS_AF_INET4;

		socklen_t xSize = sizeof(struct freertos_sockaddr);
		configPRINTF( ("Binding socket...\r\n") );
		xRet = FreeRTOS_bind(xSock, &xSourceAddress, xSize);
		if(xRet) {
			configPRINTF( ("Socket could not be bound: error %d\r\n", xRet) );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		struct freertos_sockaddr xDestinationAddress;
		xDestinationAddress.sin_addr = FreeRTOS_inet_addr(DST_ADDR_4);
		xDestinationAddress.sin_port = FreeRTOS_htons(DST_PORT);
		xDestinationAddress.sin_family = FREERTOS_AF_INET4; //FREERTOS_AFxy_INET(4,3);

		/* -- connect socket -- */
		configPRINTF( ("Connecting socket...\r\n") );
		xRet = FreeRTOS_connect(xSock, &xDestinationAddress, xSize);
		if(xRet) {
			configPRINTF( ("Socket could not be connected: error %d\r\n", xRet) );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		char cMsg[32];

		for(int i = 0; ; ++i) {
			sprintf(cMsg, "Hello world n.%d", i);
			configPRINTF( ("Sending message \"%s\"...", cMsg) );

			xRet = FreeRTOS_send(xSock, cMsg, configMIN(sizeof(cMsg), strlen(cMsg)), 0);

			configPRINTF( ("sent %d bytes.\r\n", xRet) );
			configASSERT(xRet >= 0);
			HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
			vTaskDelay(5000UL);
			//vTaskDelay(1000UL / portTICK_PERIOD_MS);
		}
	}
}

#if 0
void task_(void *argument)
{
	BaseType_t xRet;

	configPRINTF( ("Running StartDefaultTask function\r\n") );
	for(;;) {

		configPRINTF( ("Creating socket...\r\n") );
		Socket_t xSock = FreeRTOS_socket(FREERTOS_AF_INET4,
				//FREERTOS_SOCK_STREAM, 
				//FREERTOS_IPPROTO_TCP);
				 FREERTOS_SOCK_DGRAM, 
				 FREERTOS_IPPROTO_UDP);
		if(xSock == FREERTOS_INVALID_SOCKET) {
			HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
			configPRINTF( ("Socket error\r\n") );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		struct freertos_sockaddr xSourceAddress;
		//xSourceAddress.sin_addr = FreeRTOS_inet_addr_quick( ucIPAddress[ 0 ], ucIPAddress[ 1 ], ucIPAddress[ 2 ], ucIPAddress[ 3 ] );
		xSourceAddress.sin_port = FreeRTOS_htons(SRC_PORT);
		xSourceAddress.sin_family = FREERTOS_AF_INET4;

		socklen_t xSize = sizeof(struct freertos_sockaddr);
		configPRINTF( ("Binding socket...\r\n") );
		xRet = FreeRTOS_bind(xSock, /*&xSourceAddress*/ NULL, xSize);
		if(xRet) {
			configPRINTF( ("Socket could not be bound: error %d\r\n", xRet) );
			for(;;);
		}
		configPRINTF( ("Done!\r\n") );

		struct freertos_sockaddr xDestinationAddress;
		xDestinationAddress.sin_addr = FreeRTOS_inet_addr(DST_ADDR_4);
		//xDestinationAddress.sin_addr = FreeRTOS_inet_addr("192.168.56.1");
		//xDestinationAddress.sin_addr = FreeRTOS_inet_addr("255.255.255.255");
		xDestinationAddress.sin_port = FreeRTOS_htons(DST_PORT);
		xDestinationAddress.sin_family = FREERTOS_AFxy_INET(4,3);

		/*
		   configPRINTF( ("Connecting socket...\r\n") );
		   xRet = FreeRTOS_connect(xSock, &xDestinationAddress, xSize);
		   if(xRet) {
		   configPRINTF( ("Socket could not be connected: error %d\r\n", xRet) );
		   for(;;);
		   }
		   configPRINTF( ("Done!\r\n") );
		   */
		char cMsg[32];

		for(int i = 0; ; ++i) {
			sprintf(cMsg, "Hello world n.%d", i);
			configPRINTF( ("Sending message \"%s\"...", cMsg) );

			xRet = FreeRTOS_sendto(xSock, cMsg, configMIN(sizeof(cMsg), strlen(cMsg)), 0,
					&xDestinationAddress, sizeof(xDestinationAddress)
					);
			/*
			   xRet = FreeRTOS_send(xSock, cMsg, configMIN(sizeof(cMsg), strlen(cMsg)), 0);
			   */
			configPRINTF( ("sent %d bytes.\r\n", xRet) );
			configASSERT(xRet >= 0);
			HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
			vTaskDelay(1000UL);
			//vTaskDelay(1000UL / portTICK_PERIOD_MS);
		}
	}
}
#endif
