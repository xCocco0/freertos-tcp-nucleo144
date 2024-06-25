/**
 * @file main_clocksync.c
 * @brief This file contains the implementation of clock synchronization using FreeRTOS and FreeRTOS TSN.
 *
 * The code in this file demonstrates clock synchronization between a master and a slave device using FreeRTOS and FreeRTOS TSN.
 * It includes functions for sending and receiving synchronization messages, as well as handling timestamps.
 * The master device sends a sync message to the slave device, and the slave device responds with a follow-up message.
 * The code also includes functions for sending and receiving ping messages between the devices.
 *
 * @note This code assumes the use of IPv4 addresses and UDP sockets.
 * @note The IP addresses and MAC addresses used in this code are placeholders and should be replaced with actual addresses.
 *
 */

#include "main.h"

#include "FreeRTOS.h"

#include "FreeRTOS_IP.h"
#include "FreeRTOS_ARP.h"

#include "FreeRTOS_TSN_Sockets.h"
#include "FreeRTOS_TSN_Timestamp.h"

#define PORT 10001
#define PORT_PING 10002
#define MASTER_IP "169.254.174.43"
#define SLAVE_IP "169.254.174.44"
#define MASTER_MAC { 0x00, 0x80, 0xE1, 0x00, 0x00, 0x01 }
#define SLAVE_MAC { 0x00, 0x80, 0xE1, 0x00, 0x00, 0x02 }

#define HANDLE_ERROR( var, cond ) \
		if( cond ) \
		{ \
			configPRINTF(( "Error on "__FILE__":%d\r\n", __LINE__)); \
			configPRINTF(( "Returned %d\r\n", var )); \
			for( ; ; ); \
		}

/// @brief Sends a ping message to the specified address.
/// @param pxAddr Pointer to the address structure.
void vPing( struct freertos_sockaddr * const pxAddr )
{
	/// Create a socket for sending and receiving ping messages.
	Socket_t xSocket = FreeRTOS_socket( FREERTOS_AF_INET4, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );

	/// Check if the socket was created successfully.
	struct freertos_sockaddr xSourceAddr, xDestinationAddr = *pxAddr;
	/// Set the source and destination addresses for the ping message.
	xSourceAddr.sin_port = FreeRTOS_htons( PORT_PING );
	/// Set the source address based on the destination address.
	xSourceAddr.sin_family = FREERTOS_AF_INET4;
	/// If the destination address is the master device, set the source address to the slave device.
	if( pxAddr->sin_addr == FreeRTOS_inet_addr( MASTER_IP ) )
	{
		xSourceAddr.sin_addr = FreeRTOS_inet_addr( SLAVE_IP );
	}
	/// If the destination address is the slave device, set the source address to the master device.
	else
	{
		xSourceAddr.sin_addr = FreeRTOS_inet_addr( MASTER_IP );
	}
	/// Set the destination address for the ping message.
	xDestinationAddr.sin_port = FreeRTOS_htons( PORT_PING );

	/// Bind the socket to the source address.
	FreeRTOS_bind( xSocket, &xSourceAddr, sizeof( xSourceAddr ) );

	/// Set a timeout for sending the ping message.
	TickType_t xTimeout = pdMS_TO_TICKS( 10000 );
	/// Set the timeout option for the socket.
	FreeRTOS_setsockopt( xSocket, 0, FREERTOS_SO_SNDTIMEO, &xTimeout, sizeof( xTimeout ) );	
	char cMsg[] = "hello";
	/// Send the ping message to the destination address.
	BaseType_t xRet = FreeRTOS_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xDestinationAddr, sizeof( xDestinationAddr ) );
	configPRINTF(("Ping request\r\n"));

	vTaskDelay(1000);
	/// Send the ping message to the destination address.
	xRet = FreeRTOS_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xDestinationAddr, sizeof( xDestinationAddr ) );
	vTaskDelay(1000);
	/// Send the ping message to the destination address.
	xRet = FreeRTOS_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xDestinationAddr, sizeof( xDestinationAddr ) );

	/// Receive the ping reply from the destination address.
	xRet = FreeRTOS_recvfrom( xSocket, cMsg, sizeof( cMsg ), 0, NULL, NULL );
	configPRINTF(("Ping received reply\r\n"));

	vTaskDelay( 1000 );
}

/// @brief Task for the master device to send synchronization messages.
/// @param pvArgument 
void vTaskSyncMaster( void * pvArgument )
{
	configPRINTF(("Hey I am the master\r\n" ));

	/// Create a socket for sending and receiving synchronization messages.
	TSNSocket_t xSocket = FreeRTOS_TSN_socket( FREERTOS_AF_INET4, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	/// Check if the socket was created successfully.
	HANDLE_ERROR(
	xSocket, xSocket == NULL || xSocket == FREERTOS_TSN_INVALID_SOCKET );

	/// Set the timestamping options for the socket.
	BaseType_t xRet;
	uint32_t ulOpt = SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE;
	xRet = FreeRTOS_TSN_setsockopt( xSocket, FREERTOS_SOL_SOCKET, FREERTOS_SO_TIMESTAMPING, &ulOpt, sizeof( ulOpt ) );
	HANDLE_ERROR( xRet, xRet != 0 );
	
	char cMsg[ 32 ];

	/// Set the source and destination addresses for the synchronization messages.
	struct freertos_sockaddr xSlaveAddr, xMasterAddr, xSourceAddr;
	xMasterAddr.sin_port = FreeRTOS_htons( PORT );
	xMasterAddr.sin_addr = FreeRTOS_inet_addr( MASTER_IP );
	xMasterAddr.sin_family = FREERTOS_AF_INET4;
	xSlaveAddr.sin_port = FreeRTOS_htons( PORT );
	xSlaveAddr.sin_addr = FreeRTOS_inet_addr( SLAVE_IP );
	xSlaveAddr.sin_family = FREERTOS_AF_INET4;

/*
    NetworkEndPoint_t * pxEndPoint = FreeRTOS_FindEndPointOnNetMask( xSlaveAddr.sin_address.ulIP_IPv4, 10 );
    vARPRefreshCacheEntry( NULL, xSlaveAddr.sin_address.ulIP_IPv4, pxEndPoint );

	while( ! xIsIPInARPCache( xSlaveAddr.sin_addr ) )
	{
		vTaskDelay( 1000U );
	}
*/
	xRet = FreeRTOS_TSN_bind( xSocket, &xMasterAddr, sizeof( xMasterAddr ) );
	HANDLE_ERROR( xRet, xRet != 0 );

	vPing( &xSlaveAddr );

	configPRINTF(( "Sending sync\r\n" ));

	sprintf( cMsg, "Sync");
	xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xSlaveAddr, sizeof( xSlaveAddr ) );
	HANDLE_ERROR( xRet, xRet <= 0 );

    char iobuf[ 128 ];
    union
    {
        struct msghdr alignment;
        char data[ 128 ];
    } cbuf;
    struct iovec xIovec =
    {
        .iov_base = iobuf,
        .iov_len  = sizeof( iobuf )
    };
    struct msghdr xMsghdr =
    {
        .msg_name       = &xSourceAddr,
        .msg_namelen    = sizeof( xSourceAddr ),
        .msg_iov        = &xIovec,
        .msg_iovlen     = 1,
        .msg_control    = &cbuf.data,
        .msg_controllen = sizeof( cbuf ),
    };
	struct freertos_timespec ts[3];
	
	/// Receive the follow-up message from the slave device.
	do
	{
		xRet = FreeRTOS_TSN_recvmsg( xSocket, &xMsghdr, FREERTOS_MSG_ERRQUEUE );
		HANDLE_ERROR( xRet, xRet < 0 && xRet != -pdFREERTOS_ERRNO_EWOULDBLOCK );

		configPRINTF(( "Received ancillary with ret %d\r\n", xRet ));

		/// If the message is not received, wait for 500 ms and try again.
		if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
		{
			vTaskDelay( 500 );
			continue;
		}

		/// Extract the timestamp from the message.
		for( struct cmsghdr * cmsg = CMSG_FIRSTHDR( &xMsghdr ); cmsg; cmsg = CMSG_NXTHDR( &xMsghdr, cmsg ) )
		{
			if( cmsg->cmsg_level != FREERTOS_SOL_SOCKET )
				continue;

			switch( cmsg->cmsg_type )
			{
				case FREERTOS_SO_TIMESTAMPING:
						memcpy( ts, CMSG_DATA( cmsg ), sizeof( ts ) );
						configPRINTF(( "Acquired ts: %lu, %lu\r\n", ts[0].tv_sec, ts[0].tv_nsec ));
					break;

				default:
					break;
			}
		}
	}
	while( 0 );

	configPRINTF(( "Sending follow-up\r\n" ) );
	
	/// Send the follow-up message to the slave device.
	sprintf( cMsg, "Followup %lu,%lu", ts[0].tv_sec, ts[0].tv_nsec );
	xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xSlaveAddr, sizeof( xSlaveAddr ) );
	HANDLE_ERROR( xRet, xRet < 0 );

	while( 1 )
	/// Receive the delay request message from the slave device.
	{
		xMsghdr.msg_namelen = sizeof( xSourceAddr );
		xMsghdr.msg_iovlen = 1;
		xMsghdr.msg_controllen = sizeof( cbuf );
		xIovec.iov_len = sizeof( iobuf );

		xRet = FreeRTOS_TSN_recvmsg( xSocket, &xMsghdr, 0 );
		HANDLE_ERROR( xRet, xRet < 0 );

		configPRINTF(("Received: %s\r\n", iobuf ));

		if( strncmp( iobuf, "Delayreq", 8) == 0 )
		{
			for( struct cmsghdr * cmsg = CMSG_FIRSTHDR( &xMsghdr ); cmsg; cmsg = CMSG_NXTHDR( &xMsghdr, cmsg ) )
			{
				if( cmsg->cmsg_level != FREERTOS_SOL_SOCKET )
					continue;

				switch( cmsg->cmsg_type )
				{
					case FREERTOS_SO_TIMESTAMPING:
						memcpy( ts, CMSG_DATA( cmsg ), sizeof( ts ) );
						configPRINTF(( "Received ts: %lu, %lu\r\n", ts[0].tv_sec, ts[0].tv_nsec ));
						break;

					default:
						break;
				}
			}
			break;
		}
	} 

	/// Send the delay response message to the slave device.
	configPRINTF(( "Sending delay resp\r\n"));
	sprintf( cMsg, "Delayresp %lu,%lu", ts[0].tv_sec, ts[0].tv_nsec );
	xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xSlaveAddr, sizeof( xSlaveAddr ) );
	HANDLE_ERROR( xRet, xRet < 0 );

	return;
}

/// @brief Task for the slave device to receive synchronization messages.	
/// @param pvArgument 
void vTaskSyncSlave( void * pvArgument )
{
	configPRINTF(("Hello there, I am the slave\r\n" ));

	/// Create a socket for sending and receiving synchronization messages.
	TSNSocket_t xSocket = FreeRTOS_TSN_socket( FREERTOS_AF_INET4, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	HANDLE_ERROR( xSocket, xSocket == NULL || xSocket == FREERTOS_TSN_INVALID_SOCKET );

	/// Set the timestamping options for the socket.
	BaseType_t xRet;
	uint32_t ulOpt = SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE;
	xRet = FreeRTOS_TSN_setsockopt( xSocket, FREERTOS_SOL_SOCKET, FREERTOS_SO_TIMESTAMPING, &ulOpt, sizeof( ulOpt ) );
	HANDLE_ERROR( xRet, xRet != 0 );
	
	char cMsg[ 32 ];

	/// Set the source and destination addresses for the synchronization messages.
	struct freertos_sockaddr xSlaveAddr, xMasterAddr, xSourceAddr;
	uint32_t uxSourceAddrLen;
	xMasterAddr.sin_port = FreeRTOS_htons( PORT );
	xMasterAddr.sin_addr = FreeRTOS_inet_addr( MASTER_IP );
	xMasterAddr.sin_family = FREERTOS_AF_INET4;
	xSlaveAddr.sin_port = FreeRTOS_htons( PORT );
	xSlaveAddr.sin_addr = FreeRTOS_inet_addr( SLAVE_IP );
	xSlaveAddr.sin_family = FREERTOS_AF_INET4;

	/*
    NetworkEndPoint_t * pxEndPoint = FreeRTOS_FindEndPointOnNetMask( xMasterAddr.sin_address.ulIP_IPv4, 10 );
    vARPRefreshCacheEntry( NULL, xMasterAddr.sin_address.ulIP_IPv4, pxEndPoint );

	while( ! xIsIPInARPCache( xSlaveAddr.sin_addr ) )
	{
		vTaskDelay( 1000U );
	}
*/
	xRet = FreeRTOS_TSN_bind( xSocket, &xSlaveAddr, sizeof( xSlaveAddr ) );
	HANDLE_ERROR( xRet, xRet != 0 );

	/* 
	* Send a ping to the master device to establish a connection.
	*/
	vPing( &xMasterAddr );

	/*
	* Receive sync message from the master device.
	*/
    char iobuf[ 128 ];
    union
    {
        struct msghdr alignment;
        char data[ 128 ];
    } cbuf;
    struct iovec xIovec =
    {
        .iov_base = iobuf,
        .iov_len  = sizeof( iobuf )
    };
    struct msghdr xMsghdr =
    {
        .msg_name       = &xSourceAddr,
        .msg_namelen    = sizeof( xSourceAddr ),
        .msg_iov        = &xIovec,
        .msg_iovlen     = 1,
        .msg_control    = &cbuf.data,
        .msg_controllen = sizeof( cbuf ),
    };
	struct freertos_timespec ts1, ts2[3], ts3[3], ts4;
	
	while( 1 )
	{
		xMsghdr.msg_namelen = sizeof( xSourceAddr );
		xMsghdr.msg_iovlen = 1;
		xMsghdr.msg_controllen = sizeof( cbuf );
		xIovec.iov_len = sizeof( iobuf );

		/// Receive the sync message from the master device.
		xRet = FreeRTOS_TSN_recvmsg( xSocket, &xMsghdr, 0 );
		HANDLE_ERROR( xRet, xRet < 0 );

		configPRINTF(( "Received %s\r\n", iobuf ));

		/// If the sync message is received, extract the timestamp from the message.
		if( strncmp( iobuf, "Sync", 4) == 0 )
		{
			for( struct cmsghdr * cmsg = CMSG_FIRSTHDR( &xMsghdr ); cmsg; cmsg = CMSG_NXTHDR( &xMsghdr, cmsg ) )
			{
				if( cmsg->cmsg_level != FREERTOS_SOL_SOCKET )
					continue;

				switch( cmsg->cmsg_type )
				{
					case FREERTOS_SO_TIMESTAMPING:
						memcpy( ts2, CMSG_DATA( cmsg ), sizeof( ts2 ) );
						configPRINTF(( "Received ts: %lu, %lu\r\n", ts2[0].tv_sec, ts2[0].tv_nsec ));
						break;

					default:
						break;
				}
			}
			break;
		}
	}

	while( 1 )
	{
		xRet = FreeRTOS_TSN_recvfrom( xSocket, cMsg, sizeof( cMsg ), 0, &xSourceAddr, &uxSourceAddrLen );
		HANDLE_ERROR( xRet, xRet < 0 );

		configPRINTF(( "Received: %s\r\n", cMsg ));

		if( strncmp( cMsg, "Followup", 8) == 0 )
		{
			configPRINTF(( "Received %s\r\n", cMsg ));
			sscanf( cMsg, "Followup %lu,%lu", &ts1.tv_sec, &ts1.tv_nsec );
			break;
		}
	}

	configPRINTF(( "Sending delay-req\r\n"));
	sprintf( cMsg, "Delayreq" );
	xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xMasterAddr, sizeof( xMasterAddr ) );
	HANDLE_ERROR( xRet, xRet < 0 );

	do
	{
		xMsghdr.msg_namelen = sizeof( xSourceAddr );
		xMsghdr.msg_iovlen = 1;
		xMsghdr.msg_controllen = sizeof( cbuf );
		xIovec.iov_len = sizeof( iobuf );

		xRet = FreeRTOS_TSN_recvmsg( xSocket, &xMsghdr, FREERTOS_MSG_ERRQUEUE );
		HANDLE_ERROR( xRet, xRet < 0 && xRet != -pdFREERTOS_ERRNO_EWOULDBLOCK );

		configPRINTF(( "Received %s\r\n", iobuf ));

		if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
		{
			vTaskDelay( 500 );
			continue;
		}
		for( struct cmsghdr * cmsg = CMSG_FIRSTHDR( &xMsghdr ); cmsg; cmsg = CMSG_NXTHDR( &xMsghdr, cmsg ) )
		{
			if( cmsg->cmsg_level != FREERTOS_SOL_SOCKET )
				continue;

			switch( cmsg->cmsg_type )
			{
				case FREERTOS_SO_TIMESTAMPING:
					memcpy( ts3, CMSG_DATA( cmsg ), sizeof( ts3 ) );
					configPRINTF(( "Received ts: %lu, %lu\r\n", ts3[0].tv_sec, ts3[0].tv_nsec ));
					break;

				default:
					break;
			}
		}

	} while( 0 );

	while( 1 )
	{
		xRet = FreeRTOS_TSN_recvfrom( xSocket, cMsg, sizeof( cMsg ), 0, &xSourceAddr, &uxSourceAddrLen );
		HANDLE_ERROR( xRet, xRet < 0 );

		configPRINTF(( "Received: %s\r\n", cMsg ));

		if( strncmp( cMsg, "Delayresp", 9) == 0 )
		{
			sscanf( cMsg, "Delayresp %lu,%lu", &ts4.tv_sec, &ts4.tv_nsec );
			break;
		}
	}

	return;
}
