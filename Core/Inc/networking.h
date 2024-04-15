
/**
  ******************************************************************************
  * File Name          : networking.h
  * Description        : Code for freertos-plus-tcp
  ******************************************************************************
  */

#ifndef __NETWORKING_H
#define __NETWORKING_H

/*
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent );

uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
				uint16_t usSourcePort,
				uint32_t ulDestinationAddress,
				uint16_t usDestinationPort );
*/

BaseType_t xApplicationGetRandomNumber( uint32_t *pulValue );

#endif /* __NETWORKING_H */



