
#ifndef IP_TRACE_MACRO_H
#define IP_TRACE_MACRO_H

#if 1

#ifdef DEBUG

#define iptraceFAILED_TO_OBTAIN_NETWORK_BUFFER() \
	configPRINTF( ("[Trace] Failed to obtain network buffer\r\n") )
#define iptraceFAILED_TO_OBTAIN_NETWORK_BUFFER_FROM_ISR() \
	configPRINTF( ("[Trace] Failed to obtain network buffer from ISR\r\n") )
#define iptraceNETWORK_BUFFER_OBTAINED( pxBufferAddress ) \
	configPRINTF( ("[Trace] Network buffer obtained at %p\r\n", pxBufferAddress) )
#define iptraceNETWORK_BUFFER_OBTAINED_FROM_ISR( pxBufferAddress ) \
	configPRINTF( ("[Trace] Network buffer obtained from ISR at %p\r\n", pxBufferAddress) )
#define iptraceNETWORK_BUFFER_RELEASED( pxBufferAddress ) \
	configPRINTF( ("[Trace] Network buffer release at %p\r\n", pxBufferAddress) )
#define iptraceNETWORK_DOWN() \
	configPRINTF( ("[Trace] Network connection has been lost\r\n") )
#define iptraceNETWORK_EVENT_RECEIVED( eEvent ) do { \
	if( eEvent == eNoEvent) break; \
	configPRINTF( ("[Trace] Network event received: ") ); \
	switch(eEvent) { \
		case eNetworkDownEvent: configPRINTF( ("The network interface has been lost and/or needs [re]connecting") ); break; \
		case eNetworkRxEvent: configPRINTF( ("The network interface has queued a received Ethernet frame") ); break; \
		case eNetworkTxEvent: configPRINTF( ("Let the IP task send a network packet") ); break; \
		case eARPTimerEvent: configPRINTF( ("The ARP timer expired") ); break; \
		case eStackTxEvent: configPRINTF( ("The software stack has queued a packet to transmit") ); break; \
		case eDHCPEvent: configPRINTF( ("Process DHCP state machine") ); break; \
		case eTCPTimerEvent: configPRINTF( ("See if any TCP socket needs attention") ); break; \
		case eTCPAcceptEvent: configPRINTF( ("Client API accept waiting for client connections") ); break; \
		case eTCPNetStat: configPRINTF( ("IP task is asked to produce a netstat listing") ); break; \
		case eSocketBindEvent: configPRINTF( ("Send a message to the IP task to bind a socket to port") ); break; \
		case eSocketCloseEvent: configPRINTF( ("Send a message to the IP task to close a socket") ); break; \
		case eSocketSelectEvent: configPRINTF( ("Send a message to the IP task for select") ); break; \
		case eSocketSignalEvent: configPRINTF( ("A socket must be signalled") ); break; \
		case eSocketSetDeleteEvent: configPRINTF( ("A socket must be deleted") ); break; \
		default: \
	} \
	configPRINTF( ("\r\n") ); \
	} while (0)
#define iptraceNETWORK_INTERFACE_INPUT( uxDataLength, pucEthernetBuffer ) \
	configPRINTF( ("[Trace] Received packet of length %d, loaded at %p\r\n", uxDataLength, pucEthernetBuffer) )
#define iptraceNETWORK_INTERFACE_OUTPUT( uxDataLength, pucEthernetBuffer ) \
	configPRINTF( ("[Trace] Sent packet of length %d, loaded at %p\r\n", uxDataLength, pucEthernetBuffer) )
#define iptraceNETWORK_INTERFACE_RECEIVE() \
	configPRINTF( ("[Trace] Network interface received packet\r\n") )
#define iptraceNETWORK_INTERFACE_TRANSMIT() \
	configPRINTF( ("[Trace] Network interface transmitted packet\r\n") )
#define iptraceSTACK_TX_EVENT_LOST( xEvent ) \
	configPRINTF( ("[Trace] Dropped packet because of insufficient space in network event queue\r\n") )
#define iptraceETHERNET_RX_EVENT_LOST() \
	configPRINTF( ("[Trace] Ethernet packet dropped\r\n") )
#define iptraceWAITING_FOR_TX_DMA_DESCRIPTOR() \
	configPRINTF( ("[Trace] Transmission waiting for DMA\r\n") )
#define iptraceSENDING_UDP_PACKET( ulIPAddress ) \
	do { \
		char pucBuffer[16]; \
		FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer ); \
		configPRINTF( ("[Trace] Sending UDP packet to %s\r\n", pucBuffer) ); \
	} while(0)
#define iptraceBIND_FAILED( xSocket, usPort ) \
	configPRINTF( ("[Trace] Failed socket bind for port %d\r\n", FreeRTOS_ntohs(usPort)) )
#define iptraceFAILED_TO_CREATE_EVENT_GROUP() \
	configPRINTF( ("[Trace] Failed to create event group\r\n") )
#define iptraceFAILED_TO_CREATE_SOCKET() \
	configPRINTF( ("[Trace] Failed to create socket\r\n") )
#define iptraceFAILED_TO_NOTIFY_SELECT_GROUP( xSocket ) \
	configPRINTF( ("[Trace] Failed to notify select group\r\n") )
#define iptraceNO_BUFFER_FOR_SENDTO() \
	configPRINTF( ("[Trace] No buffer for sendtoa\r\n") )
#define iptraceRECVFROM_DISCARDING_BYTES( xNumberOfBytesDiscarded ) \
	configPRINTF( ("[Trace] Recvfrom discarded %d bytes\r\n", xNumberOfBytesDiscarded) )
#define iptraceRECVFROM_INTERRUPTED() \
	configPRINTF( ("[Trace] Recvfrom interrupted through FreeRTOS_SignalSocket\r\n") )
#define iptraceRECVFROM_TIMEOUT() \
	configPRINTF( ("[Trace] Recvfrom timeout\r\n") )
#define iptraceSENDTO_DATA_TOO_LONG() \
	configPRINTF( ("[Trace] Sendto data too long\r\n") )
#define iptraceSENDTO_SOCKET_NOT_BOUND() \
	configPRINTF( ("[Trace] Sendto socket not bound\r\n") )
#define traceARP_PACKET_RECEIVED() \
	configPRINTF( ("[Trace] ARP Packet received\r\n") )
#define iptraceARP_TABLE_ENTRY_CREATED( ulIPAddress, ucMACAddress ) \
	do { \
		char pucBuffer[16]; \
		FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer ); \
		configPRINTF( ("[Trace] ARP Table entry crated for IP %s\r\n", pucBuffer) ); \
	} while(0)
#define iptraceARP_TABLE_ENTRY_EXPIRED( ulIPAddress ); \
	do {\
		char pucBuffer[16]; \
		FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer ); \
		configPRINTF( ("[Trace] ARP Table entry expired for %s\r\n", pucBuffer) );\
	} while(0)
#define iptraceARP_TABLE_ENTRY_WILL_EXPIRE( ulIPAddress ) \
	do {\
		char pucBuffer[16]; \
		FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer ); \
		configPRINTF( ("[Trace] ARP Table entry will expire for %s\r\n", pucBuffer) );\
	} while(0)
#define iptraceCREATING_ARP_REQUEST( ulIPAddress ) \
	do { \
		char pucBuffer[16]; \
		FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer ); \
		configPRINTF( ("[Trace] Creating ARP Request %s\r\n", pucBuffer) ); \
	} while(0)
#define iptraceDELAYED_ARP_BUFFER_FULL() \
	configPRINTF( ("[Trace] Delayed ARP buffer full\r\n") )
#define iptrace_DELAYED_ARP_REQUEST_REPLIED() \
	configPRINTF( ("[Trace] Delayed ARP request replied\r\n") )
#define iptraceDELAYED_ARP_REQUEST_STARTED() \
	configPRINTF( ("[Trace] Delayed ARP request started\r\n") )
#define iptraceDELAYED_ARP_TIMER_EXPIRED() \
	configPRINTF( ("[Trace] Delayed ARP timer expired\r\n") )
#define iptraceDROPPED_INVALID_ARP_PACKET( pxARPHeader ) \
	configPRINTF( ("[Trace] Dropped invalid ARP packet at %p\r\n", pxARPHeader) )
#define iptracePACKET_DROPPED_TO_GENERATE_ARP( ulIPAddress ) \
	do { \
		char pucBuffer[16]; \
		FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer ); \
		configPRINTF( ("[Trace] Packet for %s dropped to generate ARP\r\n", pucBuffer) ); \
	} while(0)
#define iptracePROCESSING_RECEIVED_ARP_REPLY( ulIPAddress ) \
	do { \
		char pucBuffer[16]; \
		FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer ); \
		configPRINTF( ("[Trace] Processing received ARP reply from %s\r\n", pucBuffer) ); \
	} while(0)
#define iptraceSENDING_ARP_REPLY( ulIPAddress ) \
	do { \
		char pucBuffer[16]; \
		FreeRTOS_inet_ntoa( ulIPAddress, pucBuffer ); \
		configPRINTF( ("[Trace] Sending ARP reply for %s\r\n", pucBuffer) ); \
	} while(0)

#if 0 //TBD
#define iptraceDHCP_REQUESTS_FAILED_USING_DEFAULT_IP_ADDRESS( ulIPAddress ) configPRINTF( ("[Trace]\r\n") )
#define iptraceDHCP_REQUESTS_FAILED_USING_DEFAULT_IPv6_ADDRESS( xIPAddress ) configPRINTF( ("[Trace]\r\n") )
#define iptraceDHCP_SUCCEEDED( ulOfferedIPAddress ) configPRINTF( ("[Trace]\r\n") )
#define iptraceSENDING_DHCP_DISCOVER() configPRINTF( ("[Trace]\r\n") )
#define iptraceSENDING_DHCP_REQUEST() configPRINTF( ("[Trace]\r\n") )
#define iptraceSENDING_DNS_REQUEST() configPRINTF( ("[Trace]\r\n") )
#define iptraceICMP_PACKET_RECEIVED() configPRINTF( ("[Trace]\r\n") )
#define iptraceSENDING_PING_REPLY( ulIPAddress ) configPRINTF( ("[Trace]\r\n") )
#define iptraceND_TABLE_ENTRY_EXPIRED( pxIPAddress ) configPRINTF( ("[Trace]\r\n") )
#define iptraceND_TABLE_ENTRY_WILL_EXPIRE( pxIPAddress ) configPRINTF( ("[Trace]\r\n") )
#define iptraceRA_REQUESTS_FAILED_USING_DEFAULT_IP_ADDRESS( ipv6_address ) configPRINTF( ("[Trace]\r\n") )
#define iptraceRA_SUCCEEDED( ipv6_address ) configPRINTF( ("[Trace]\r\n") )
#define iptraceMEM_STATS_CLOSE() configPRINTF( ("[Trace]\r\n") )
#define iptraceMEM_STATS_CREATE( xMemType, pxObject, uxSize ) configPRINTF( ("[Trace]\r\n") )
#define iptraceMEM_STATS_DELETE( pxObject ) configPRINTF( ("[Trace]\r\n") )
#define iptraceDUMP_INIT( pcFileName, pxEntries ) configPRINTF( ("[Trace]\r\n") )
#define iptraceDUMP_PACKET( pucBuffer, uxLength, xIncoming ) configPRINTF( ("[Trace]\r\n") )
#endif

#endif /* DEBUG */

#endif

#endif /* IP_TRACE_MACRO_H */

