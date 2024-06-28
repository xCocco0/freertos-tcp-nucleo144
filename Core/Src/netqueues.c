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
BaseType_t Port10001( NetworkBufferDescriptor_t * pxNetworkBuffer )
{
    return ( ( UDPPacket_t * ) pxNetworkBuffer->pucEthernetBuffer )->xUDPHeader.usDestinationPort == FreeRTOS_htons( 10001 );
}

BaseType_t Port10003( NetworkBufferDescriptor_t * pxNetworkBuffer )
{
    return ( ( UDPPacket_t * ) pxNetworkBuffer->pucEthernetBuffer )->xUDPHeader.usDestinationPort == FreeRTOS_htons( 10003 );
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
    NetworkQueue_t * queue1, * queue2_r, * queue2_s, * queue3;

    queue1 = pxNetworkQueueCreate( eSendRecv, configMAX_PRIORITIES - 1, "queue1", Port10001 );
    queue2_s = pxNetworkQueueCreate( eSendOnly, 1, "queue2_s", Port10003 );
    queue2_r = pxNetworkQueueCreate( eRecvOnly, 1, "queue2_r", Port10003 );
    queue3 = pxNetworkQueueCreate( eIPTaskEvents, 0, "queue3", NULL );

    NetworkNode_t * prio, * fifo, * fifo2, * cbs, * fifoHP, * prio_cbs;

    fifoHP = pxNetworkNodeCreateFIFO();
    ( void ) xNetworkSchedulerLinkQueue( fifoHP, queue1 );

    cbs = pxNetworkNodeCreateCBS( 57 * 8, 57 * 8 * 2 );
    ( void ) xNetworkSchedulerLinkQueue( cbs, queue2_s );

    fifo2 = pxNetworkNodeCreateFIFO();
    ( void ) xNetworkSchedulerLinkQueue( fifo2, queue2_r );

    fifo = pxNetworkNodeCreateFIFO();
    ( void ) xNetworkSchedulerLinkQueue( fifo, queue3 );

    prio_cbs = pxNetworkNodeCreatePrio( 2 );
    ( void ) xNetworkSchedulerLinkChild( prio_cbs, cbs, 0 );
    ( void ) xNetworkSchedulerLinkChild( prio_cbs, fifo2, 1 );

    prio = pxNetworkNodeCreatePrio( 3 );
    ( void ) xNetworkSchedulerLinkChild( prio, fifoHP, 0 );
    ( void ) xNetworkSchedulerLinkChild( prio, prio_cbs, 1 );
    ( void ) xNetworkSchedulerLinkChild( prio, fifo, 2 );

    /*
     *              +-0---------------->[fifoHP]->queue1
     *              |
     *              |               +-0->[cbs]->queue2_s
     * root->[prio]-+-1->[prio_cbs]-|
     *              |               +-1->[fifo2]->queue2_r
     *              |
     *              +-2---------------->[fifo]->queue3
     */

    ( void ) xNetworkQueueAssignRoot( prio );
}
