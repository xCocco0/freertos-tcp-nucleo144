#ifndef IP_TRACE_MACRO_H
#define IP_TRACE_MACRO_H

#define iptracePRINT_FUNC( X )    vLoggerPrint X

#if 1

    #ifdef DEBUG

        #define iptraceFAILED_TO_OBTAIN_NETWORK_BUFFER() \
    iptracePRINT_FUNC( ( "[Trace] Failed to obtain network buffer\r\n" ) )
        #define iptraceFAILED_TO_OBTAIN_NETWORK_BUFFER_FROM_ISR() \
    iptracePRINT_FUNC( ( "[Trace] Failed to obtain network buffer from ISR\r\n" ) )
        #define iptraceNETWORK_BUFFER_OBTAINED( pxBufferAddress ) \
    iptracePRINT_FUNC( ( "[Trace] Network buffer obtained at %p\r\n", pxBufferAddress ) )
        #define iptraceNETWORK_BUFFER_OBTAINED_FROM_ISR( pxBufferAddress ) \
    iptracePRINT_FUNC( ( "[Trace] Network buffer obtained from ISR at %p\r\n", pxBufferAddress ) )
        #define iptraceNETWORK_BUFFER_RELEASED( pxBufferAddress ) \
    iptracePRINT_FUNC( ( "[Trace] Network buffer release at %p\r\n", pxBufferAddress ) )
        #define iptraceNETWORK_DOWN() \
    iptracePRINT_FUNC( ( "[Trace] Network connection has been lost\r\n" ) )
        #define iptraceNETWORK_EVENT_RECEIVED( eEvent )                                                            \
    do {                                                                                                           \
        if( eEvent == eNoEvent ) break;                                                                            \
        iptracePRINT_FUNC( ( "[Trace] Network event received: " ) );                                               \
        switch( eEvent ) {                                                                                         \
            case eNetworkDownEvent:                                                                                \
                iptracePRINT_FUNC( ( "The network interface has been lost and/or needs [re]connecting" ) ); break; \
            case eNetworkRxEvent:                                                                                  \
                iptracePRINT_FUNC( ( "The network interface has queued a received Ethernet frame" ) ); break;      \
            case eNetworkTxEvent:                                                                                  \
                iptracePRINT_FUNC( ( "Let the IP task send a network packet" ) ); break;                           \
            case eARPTimerEvent:                                                                                   \
                iptracePRINT_FUNC( ( "The ARP timer expired" ) ); break;                                           \
            case eStackTxEvent:                                                                                    \
                iptracePRINT_FUNC( ( "The software stack has queued a packet to transmit" ) ); break;              \
            case eDHCPEvent:                                                                                       \
                iptracePRINT_FUNC( ( "Process DHCP state machine" ) ); break;                                      \
            case eTCPTimerEvent:                                                                                   \
                iptracePRINT_FUNC( ( "See if any TCP socket needs attention" ) ); break;                           \
            case eTCPAcceptEvent:                                                                                  \
                iptracePRINT_FUNC( ( "Client API accept waiting for client connections" ) ); break;                \
            case eTCPNetStat:                                                                                      \
                iptracePRINT_FUNC( ( "IP task is asked to produce a netstat listing" ) ); break;                   \
            case eSocketBindEvent:                                                                                 \
                iptracePRINT_FUNC( ( "Send a message to the IP task to bind a socket to port" ) ); break;          \
            case eSocketCloseEvent:                                                                                \
                iptracePRINT_FUNC( ( "Send a message to the IP task to close a socket" ) ); break;                 \
            case eSocketSelectEvent:                                                                               \
                iptracePRINT_FUNC( ( "Send a message to the IP task for select" ) ); break;                        \
            case eSocketSignalEvent:                                                                               \
                iptracePRINT_FUNC( ( "A socket must be signalled" ) ); break;                                      \
            case eSocketSetDeleteEvent:                                                                            \
                iptracePRINT_FUNC( ( "A socket must be deleted" ) ); break;                                        \
            default:                                                                                               \
                break;                                                                                             \
        }                                                                                                          \
        iptracePRINT_FUNC( ( "\r\n" ) );                                                                           \
    } while( 0 )
        #define iptraceNETWORK_INTERFACE_INPUT( uxDataLength, pucEthernetBuffer ) \
    iptracePRINT_FUNC( ( "[Trace] Received packet of length %d, loaded at %p\r\n", uxDataLength, pucEthernetBuffer ) )
        #define iptraceNETWORK_INTERFACE_OUTPUT( uxDataLength, pucEthernetBuffer ) \
    iptracePRINT_FUNC( ( "[Trace] Sent packet of length %d, loaded at %p\r\n", uxDataLength, pucEthernetBuffer ) )
        #define iptraceNETWORK_INTERFACE_RECEIVE() \
    iptracePRINT_FUNC( ( "[Trace] Network interface received packet\r\n" ) )
        #define iptraceNETWORK_INTERFACE_TRANSMIT() \
    iptracePRINT_FUNC( ( "[Trace] Network interface transmitted packet\r\n" ) )
        #define iptraceSTACK_TX_EVENT_LOST( xEvent ) \
    iptracePRINT_FUNC( ( "[Trace] Dropped packet because of insufficient space in network event queue\r\n" ) )
        #define iptraceETHERNET_RX_EVENT_LOST() \
    iptracePRINT_FUNC( ( "[Trace] Ethernet packet dropped\r\n" ) )
        #define iptraceWAITING_FOR_TX_DMA_DESCRIPTOR() \
    iptracePRINT_FUNC( ( "[Trace] Transmission waiting for DMA\r\n" ) )
        #define iptraceSENDING_UDP_PACKET( ulIPAddress )                            \
    do {                                                                            \
        char pucBuffer[ 16 ];                                                       \
        FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer );                               \
        iptracePRINT_FUNC( ( "[Trace] Sending UDP packet to %s\r\n", pucBuffer ) ); \
    } while( 0 )
        #define iptraceBIND_FAILED( xSocket, usPort ) \
    iptracePRINT_FUNC( ( "[Trace] Failed socket bind for port %d\r\n", FreeRTOS_ntohs( usPort ) ) )
        #define iptraceFAILED_TO_CREATE_EVENT_GROUP() \
    iptracePRINT_FUNC( ( "[Trace] Failed to create event group\r\n" ) )
        #define iptraceFAILED_TO_CREATE_SOCKET() \
    iptracePRINT_FUNC( ( "[Trace] Failed to create socket\r\n" ) )
        #define iptraceFAILED_TO_NOTIFY_SELECT_GROUP( xSocket ) \
    iptracePRINT_FUNC( ( "[Trace] Failed to notify select group\r\n" ) )
        #define iptraceNO_BUFFER_FOR_SENDTO() \
    iptracePRINT_FUNC( ( "[Trace] No buffer for sendtoa\r\n" ) )
        #define iptraceRECVFROM_DISCARDING_BYTES( xNumberOfBytesDiscarded ) \
    iptracePRINT_FUNC( ( "[Trace] Recvfrom discarded %d bytes\r\n", xNumberOfBytesDiscarded ) )
        #define iptraceRECVFROM_INTERRUPTED() \
    iptracePRINT_FUNC( ( "[Trace] Recvfrom interrupted through FreeRTOS_SignalSocket\r\n" ) )
        #define iptraceRECVFROM_TIMEOUT() \
    iptracePRINT_FUNC( ( "[Trace] Recvfrom timeout\r\n" ) )
        #define iptraceSENDTO_DATA_TOO_LONG() \
    iptracePRINT_FUNC( ( "[Trace] Sendto data too long\r\n" ) )
        #define iptraceSENDTO_SOCKET_NOT_BOUND() \
    iptracePRINT_FUNC( ( "[Trace] Sendto socket not bound\r\n" ) )
        #define traceARP_PACKET_RECEIVED() \
    iptracePRINT_FUNC( ( "[Trace] ARP Packet received\r\n" ) )
        #define iptraceARP_TABLE_ENTRY_CREATED( ulIPAddress, ucMACAddress )                 \
    do {                                                                                    \
        char pucBuffer[ 16 ];                                                               \
        FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer );                                       \
        iptracePRINT_FUNC( ( "[Trace] ARP Table entry crated for IP %s\r\n", pucBuffer ) ); \
    } while( 0 )
        #define iptraceARP_TABLE_ENTRY_EXPIRED( ulIPAddress )                             \
    ;                                                                                     \
    do {                                                                                  \
        char pucBuffer[ 16 ];                                                             \
        FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer );                                     \
        iptracePRINT_FUNC( ( "[Trace] ARP Table entry expired for %s\r\n", pucBuffer ) ); \
    } while( 0 )
        #define iptraceARP_TABLE_ENTRY_WILL_EXPIRE( ulIPAddress )                             \
    do {                                                                                      \
        char pucBuffer[ 16 ];                                                                 \
        FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer );                                         \
        iptracePRINT_FUNC( ( "[Trace] ARP Table entry will expire for %s\r\n", pucBuffer ) ); \
    } while( 0 )
        #define iptraceCREATING_ARP_REQUEST( ulIPAddress )                         \
    do {                                                                           \
        char pucBuffer[ 16 ];                                                      \
        FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer );                              \
        iptracePRINT_FUNC( ( "[Trace] Creating ARP Request %s\r\n", pucBuffer ) ); \
    } while( 0 )
        #define iptraceDELAYED_ARP_BUFFER_FULL() \
    iptracePRINT_FUNC( ( "[Trace] Delayed ARP buffer full\r\n" ) )
        #define iptrace_DELAYED_ARP_REQUEST_REPLIED() \
    iptracePRINT_FUNC( ( "[Trace] Delayed ARP request replied\r\n" ) )
        #define iptraceDELAYED_ARP_REQUEST_STARTED() \
    iptracePRINT_FUNC( ( "[Trace] Delayed ARP request started\r\n" ) )
        #define iptraceDELAYED_ARP_TIMER_EXPIRED() \
    iptracePRINT_FUNC( ( "[Trace] Delayed ARP timer expired\r\n" ) )
        #define iptraceDROPPED_INVALID_ARP_PACKET( pxARPHeader ) \
    iptracePRINT_FUNC( ( "[Trace] Dropped invalid ARP packet at %p\r\n", pxARPHeader ) )
        #define iptracePACKET_DROPPED_TO_GENERATE_ARP( ulIPAddress )                             \
    do {                                                                                         \
        char pucBuffer[ 16 ];                                                                    \
        FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer );                                            \
        iptracePRINT_FUNC( ( "[Trace] Packet for %s dropped to generate ARP\r\n", pucBuffer ) ); \
    } while( 0 )
        #define iptracePROCESSING_RECEIVED_ARP_REPLY( ulIPAddress )                              \
    do {                                                                                         \
        char pucBuffer[ 16 ];                                                                    \
        FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer );                                            \
        iptracePRINT_FUNC( ( "[Trace] Processing received ARP reply from %s\r\n", pucBuffer ) ); \
    } while( 0 )
        #define iptraceSENDING_ARP_REPLY( ulIPAddress )                             \
    do {                                                                            \
        char pucBuffer[ 16 ];                                                       \
        FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer );                               \
        iptracePRINT_FUNC( ( "[Trace] Sending ARP reply for %s\r\n", pucBuffer ) ); \
    } while( 0 )

        #if 0 /*TBD */
            #define iptraceDHCP_REQUESTS_FAILED_USING_DEFAULT_IP_ADDRESS( ulIPAddress )     iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceDHCP_REQUESTS_FAILED_USING_DEFAULT_IPv6_ADDRESS( xIPAddress )    iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceDHCP_SUCCEEDED( ulOfferedIPAddress )                             iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceSENDING_DHCP_DISCOVER()                                          iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceSENDING_DHCP_REQUEST()                                           iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceSENDING_DNS_REQUEST()                                            iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceICMP_PACKET_RECEIVED()                                           iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceSENDING_PING_REPLY( ulIPAddress )                                iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceND_TABLE_ENTRY_EXPIRED( pxIPAddress )                            iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceND_TABLE_ENTRY_WILL_EXPIRE( pxIPAddress )                        iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceRA_REQUESTS_FAILED_USING_DEFAULT_IP_ADDRESS( ipv6_address )      iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceRA_SUCCEEDED( ipv6_address )                                     iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceMEM_STATS_CLOSE()                                                iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceMEM_STATS_CREATE( xMemType, pxObject, uxSize )                   iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceMEM_STATS_DELETE( pxObject )                                     iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceDUMP_INIT( pcFileName, pxEntries )                               iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
            #define iptraceDUMP_PACKET( pucBuffer, uxLength, xIncoming )                    iptracePRINT_FUNC( ( "[Trace]\r\n" ) )
        #endif /* if 0 */

    #endif /* DEBUG */

#endif /* if 1 */

#endif /* IP_TRACE_MACRO_H */
