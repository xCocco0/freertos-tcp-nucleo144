
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "cmsis_os.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "peripherals.h"

#ifdef DEBUG
#include "logger.h"
#endif

#define DST_ADDR_4 "169.254.151.41"
#define DST_ADDR_6 "fe80::4535:c3a4:9457:6e5a"
#define DST_PORT 10000

#define SRC_ADDR_4 "169.254.151.42"
#define SRC_ADDR_6 "fe80::dead:beef"
#define SRC_PORT 10000


void vTaskUDPSendIPv4(void *argument);
void vTaskUDPSendIPv6(void *argument);
void vTaskTCPSendIPv4(void *argument);
void vTaskTCPSendIPv6(void *argument);

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
		xDestinationAddress.sin_family = FREERTOS_AFxy_INET(4,3);

		char cMsg[32];

		for(int i = 0; ; ++i) {

			sprintf(cMsg, "Hello world n.%d", i);
			configPRINTF( ("Sending message \"%s\"...", cMsg) );

			xRet = FreeRTOS_sendto( xSock, cMsg, configMIN(sizeof(cMsg), strlen(cMsg)), 0,
					&xDestinationAddress, sizeof(xDestinationAddress) );
			configPRINTF( ("sent %d bytes.\r\n", xRet) );
			configASSERT(xRet >= 0);
			HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
			osDelay(5000UL);
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
			osDelay(5000UL);
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
			osDelay(5000UL);
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
			osDelay(1000UL);
			//vTaskDelay(1000UL / portTICK_PERIOD_MS);
		}
	}
}
#endif
