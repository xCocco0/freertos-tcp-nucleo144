#include <string.h>
#include "FreeRTOS_TSN_NetworkScheduler.h"
#include "BasicSchedulers.h"
#include "SchedCBS.h"

/**
 * @brief Check function that always returns true.
 *
 * @param pxNetworkBuffer Pointer to the network buffer descriptor.
 * @return pdTRUE.
 */
BaseType_t AlwaysTrue( NetworkBufferDescriptor_t * pxNetworkBuffer )
{
    return pdTRUE;
}

/**
 * @brief Check function that always returns false.
 *
 * @param pxNetworkBuffer Pointer to the network buffer descriptor.
 * @return pdFALSE.
 */
BaseType_t AlwaysFalse( NetworkBufferDescriptor_t * pxNetworkBuffer )
{
    return pdFALSE;
}

/**
 * @brief Check function that checks if the destination port of a UDP packet is 10000.
 *
 * @param pxNetworkBuffer Pointer to the network buffer descriptor.
 * @return pdTRUE if the destination port is 10000, pdFALSE otherwise.
 */
BaseType_t Port10000( NetworkBufferDescriptor_t * pxNetworkBuffer )
{
    return ( ( UDPPacket_t * ) pxNetworkBuffer->pucEthernetBuffer )->xUDPHeader.usDestinationPort == FreeRTOS_htons( 10000 );
}

/**
 * @brief Function that prints the contents of a network buffer when called.
 *
 * @param pxBuf Pointer to the network buffer descriptor.
 * @return pdPASS.
 */
BaseType_t PrintPacketIn( NetworkBufferDescriptor_t * pxBuf )
{
    /*configPRINTF( ("PacketIn: %12s\r\n", pxBuf->pucEthernetBuffer) ); */
    return pdPASS;
}

/**
 * @brief Function that prints the contents of a network buffer when called.
 *
 * @param pxBuf Pointer to the network buffer descriptor.
 * @return pdPASS.
 */
BaseType_t PrintPacketOut( NetworkBufferDescriptor_t * pxBuf )
{
    /*configPRINTF( ("PacketOut: %12s\r\n", pxBuf->pucEthernetBuffer) ); */
    return pdPASS;
}

/**
 * @brief Initializes the network queues and network nodes.
 */
void vNetworkQueueInit( void )
{
    NetworkQueue_t * queue1, * queue2, * queue3;

    queue1 = pxNetworkQueueCreate( eSendRecv, 1, "queue1", Port10000 );
    ( void ) queue2;
    queue3 = pxNetworkQueueCreate( eIPTaskEvents, 0, "queue3", NULL );

    NetworkNode_t * prio, * fifo, * cbs;

    cbs = pxNetworkNodeCreateCBS( 12 * 8, 12 * 2 * 8 );
    ( void ) xNetworkSchedulerLinkQueue( cbs, queue1 );

    fifo = pxNetworkNodeCreateFIFO();
    ( void ) xNetworkSchedulerLinkQueue( fifo, queue3 );

    prio = pxNetworkNodeCreatePrio( 2 );
    ( void ) xNetworkSchedulerLinkChild( prio, cbs, 0 );
    ( void ) xNetworkSchedulerLinkChild( prio, fifo, 1 );

    ( void ) xNetworkQueueAssignRoot( prio );
}
