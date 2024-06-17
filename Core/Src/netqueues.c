
#include <string.h>

#include "FreeRTOS_TSN_NetworkScheduler.h"

#include "BasicSchedulers.h"
#include "SchedCBS.h"

BaseType_t AlwaysTrue( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
	return pdTRUE;
}

BaseType_t AlwaysFalse( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
	return pdFALSE;
}

BaseType_t Port10000( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
	return ((UDPPacket_t *)pxNetworkBuffer->pucEthernetBuffer)->xUDPHeader.usDestinationPort == FreeRTOS_htons(10000);
}

BaseType_t PrintPacketIn( NetworkBufferDescriptor_t * pxBuf )
{
	//configPRINTF( ("PacketIn: %12s\r\n", pxBuf->pucEthernetBuffer) );
	return pdPASS;
}

BaseType_t PrintPacketOut( NetworkBufferDescriptor_t * pxBuf )
{
	//configPRINTF( ("PacketOut: %12s\r\n", pxBuf->pucEthernetBuffer) );
	return pdPASS;
}

void vNetworkQueueInit( void ) {

	NetworkQueue_t *queue1, *queue2, *queue3;

	queue1 = pxNetworkQueueCreate( eSendRecv, 1, "queue1", Port10000 );
	( void ) queue2;
	queue3 = pxNetworkQueueCreate( eIPTaskEvents, 0, "queue3", NULL );

	NetworkNode_t *prio, *fifo, *cbs;

	cbs = pxNetworkNodeCreateCBS( 12*8, 12*2*8 );
	( void ) xNetworkSchedulerLinkQueue( cbs, queue1 );

	fifo = pxNetworkNodeCreateFIFO();
	( void ) xNetworkSchedulerLinkQueue( fifo, queue3 );

	prio = pxNetworkNodeCreatePrio( 2 );
	( void ) xNetworkSchedulerLinkChild( prio, cbs, 0 );
	( void ) xNetworkSchedulerLinkChild( prio, fifo, 1 );

	( void ) xNetworkQueueAssignRoot( prio );
}




