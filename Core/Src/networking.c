
/**
  ******************************************************************************
  * File Name          : networking.c
  * Description        : Code for freertos-plus-tcp
  ******************************************************************************
  */

#include "main.h"
#include "peripherals.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

/* This is called when network is connected */
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
		uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
		char cBuffer[ 16 ];
		static BaseType_t xTasksAlreadyCreated = pdFALSE;

		/* If the network has just come up...*/
		if( eNetworkEvent == eNetworkUp )
		{
				configPRINTF( ("Network up event hook has been called\r\n") );
				/* Create the tasks that use the IP stack if they have not already been
				   created. */
				if( xTasksAlreadyCreated == pdFALSE )
				{
						/* Demos that use the network are created after the network is
						   up. */
						//			configPRINTF( ( "---------STARTING DEMO---------\r\n" ) );
						//			vStartSimpleMQTTDemo();
						xTasksAlreadyCreated = pdTRUE;
				}

				FreeRTOS_GetAddressConfiguration( &ulIPAddress, &ulNetMask, &ulGatewayAddress, &ulDNSServerAddress );
				FreeRTOS_inet_ntoa( ulIPAddress, cBuffer );
				configPRINTF( ( "IP Address: %s\r\n", cBuffer ) );

				FreeRTOS_inet_ntoa( ulNetMask, cBuffer );
				configPRINTF( ( "Subnet Mask: %s\r\n", cBuffer ) );

				FreeRTOS_inet_ntoa( ulGatewayAddress, cBuffer );
				configPRINTF( ( "Gateway Address: %s\r\n", cBuffer ) );

				FreeRTOS_inet_ntoa( ulDNSServerAddress, cBuffer );
				configPRINTF( ( "DNS Server Address: %s\r\n", cBuffer ) );
		}
}

/* Generate TCP start seq no */
uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
				uint16_t usSourcePort,
				uint32_t ulDestinationAddress,
				uint16_t usDestinationPort )
{
		uint32_t pulNumber = 0;

		xApplicationGetRandomNumber( &pulNumber );
		return pulNumber;
}

BaseType_t xApplicationGetRandomNumber( uint32_t *pulValue )
{
		HAL_StatusTypeDef xResult;
		BaseType_t xReturn;
		uint32_t ulValue;
		
		xResult = HAL_RNG_GenerateRandomNumber( &hrng, &ulValue );

		if( xResult == HAL_OK )
		{
				xReturn = pdPASS;
				*pulValue = ulValue;
		}
		else
		{
				xReturn = pdFAIL;
		}
		return xReturn;
}

