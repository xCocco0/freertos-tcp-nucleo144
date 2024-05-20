
#include <string.h>

#include "FreeRTOS_TSN_NetworkScheduler.h"

BaseType_t AlwaysTrue( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
	return pdTRUE;
}

BaseType_t AlwaysFalse( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
	return pdFALSE;
}

BaseType_t Port80( NetworkBufferDescriptor_t *pxNetworkBuffer )
{
	return pxNetworkBuffer->usPort == 80;
}

void vNetworkQueueInit() {
	
	NetworkQueue_t *queue1, *queue2;

	queue1 = pxNetworkQueueCreate();
	queue2 = pxNetworkQueueFromIPEventQueue();

	strcpy( (char *) &queue1->ucName, "queue1");
	queue1->fnFilter = Port80;

	strcpy( (char *) &queue2->ucName, "queue2");
	queue2->fnFilter = AlwaysTrue;

	NetworkQueueNode_t *prio, *fifo, *cbs;

	fifo = pxNetworkQueueNodeCreateFIFO();
	fifo->pxQueue = queue1;

	( void ) xNetworkQueueAssignRoot( fifo );
}




