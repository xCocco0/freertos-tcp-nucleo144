
#include "main.h"

#include "FreeRTOS.h"

#include "FreeRTOS_TSN_Sockets.h"
#include "FreeRTOS_TSN_Timestamp.h"

#define PORT 10001
#define MASTER_IP "169.254.174.2"
#define SLAVE_IP "169.254.174.3"

#define HANDLE_ERROR( var, cond ) \
		if( cond ) \
		{ \
			configPRINTF(( "Error on"__FILE__":%d\r\n", __LINE__)); \
			configPRINTF(( "Returned %d\r\n", var )); \
			for( ; ; ); \
		}

void vTaskSyncMaster( void * pvArgument )
{
	TSNSocket_t xSocket = FreeRTOS_TSN_socket( FREERTOS_AF_INET4, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	HANDLE_ERROR( xSocket, xSocket != NULL && xSocket != FREERTOS_TSN_INVALID_SOCKET );

	BaseType_t xRet;
	uint32_t ulOpt = SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE;
	xRet = FreeRTOS_TSN_setsockopt( xSocket, FREERTOS_SOL_SOCKET, FREERTOS_SO_TIMESTAMPING, &ulOpt, sizeof( ulOpt ) );
	HANDLE_ERROR( xRet, xRet != 0 );
	
	char cMsg[ 32 ];

	struct freertos_sockaddr xSlaveAddr, xMasterAddr, xSourceAddr;
	xMasterAddr.sin_port = FreeRTOS_htons( PORT );
	xMasterAddr.sin_addr = FreeRTOS_inet_addr( MASTER_IP );
	xMasterAddr.sin_family = FREERTOS_AF_INET4;
	xSlaveAddr.sin_port = FreeRTOS_htons( PORT );
	xSlaveAddr.sin_addr = FreeRTOS_inet_addr( SLAVE_IP );
	xSlaveAddr.sin_family = FREERTOS_AF_INET4;

	xRet = FreeRTOS_TSN_bind( xSocket, &xMasterAddr, sizeof( xMasterAddr ) );
	HANDLE_ERROR( xRet, xRet != 0 );

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
	
	do
	{
		xRet = FreeRTOS_TSN_recvmsg( xSocket, &xMsghdr, FREERTOS_MSG_ERRQUEUE );
		HANDLE_ERROR( xRet, xRet <= 0 && xRet != -pdFREERTOS_ERRNO_EWOULDBLOCK );

		configPRINTF(( "Received ancillary with ret %d\r\n", xRet ));

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

	sprintf( cMsg, "Followup %lu,%lu", ts[0].tv_sec, ts[0].tv_nsec );
	xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xSlaveAddr, sizeof( xSlaveAddr ) );
	HANDLE_ERROR( xRet, xRet <= 0 );

	do
	{
		xMsghdr.msg_namelen = sizeof( xSourceAddr );
		xMsghdr.msg_iovlen = 1;
		xMsghdr.msg_controllen = sizeof( cbuf );
		xIovec.iov_len = sizeof( iobuf );

		xRet = FreeRTOS_TSN_recvmsg( xSocket, &xMsghdr, 0 );
		HANDLE_ERROR( xRet, xRet <= 0 );

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
	} while( 1 );

	
	configPRINTF(( "Sending delay resp\r\n"));
	sprintf( cMsg, "Delayresp %lu,%lu", ts[0].tv_sec, ts[0].tv_nsec );
	xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xSlaveAddr, sizeof( xSlaveAddr ) );
	HANDLE_ERROR( xRet, xRet <= 0 );

	return;
}

void vTaskSyncSlave( void * pvArgument )
{
	TSNSocket_t xSocket = FreeRTOS_TSN_socket( FREERTOS_AF_INET4, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	HANDLE_ERROR( xSocket, xSocket != NULL && xSocket != FREERTOS_TSN_INVALID_SOCKET );

	BaseType_t xRet;
	uint32_t ulOpt = SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE;
	xRet = FreeRTOS_TSN_setsockopt( xSocket, FREERTOS_SOL_SOCKET, FREERTOS_SO_TIMESTAMPING, &ulOpt, sizeof( ulOpt ) );
	HANDLE_ERROR( xRet, xRet != 0 );
	
	char cMsg[ 32 ];

	struct freertos_sockaddr xSlaveAddr, xMasterAddr, xSourceAddr;
	uint32_t uxSourceAddrLen;
	xMasterAddr.sin_port = FreeRTOS_htons( PORT );
	xMasterAddr.sin_addr = FreeRTOS_inet_addr( MASTER_IP );
	xMasterAddr.sin_family = FREERTOS_AF_INET4;
	xSlaveAddr.sin_port = FreeRTOS_htons( PORT );
	xSlaveAddr.sin_addr = FreeRTOS_inet_addr( SLAVE_IP );
	xSlaveAddr.sin_family = FREERTOS_AF_INET4;

	xRet = FreeRTOS_TSN_bind( xSocket, &xSlaveAddr, sizeof( xSlaveAddr ) );
	HANDLE_ERROR( xRet, xRet != 0 );

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
	
	do
	{
		xMsghdr.msg_namelen = sizeof( xSourceAddr );
		xMsghdr.msg_iovlen = 1;
		xMsghdr.msg_controllen = sizeof( cbuf );
		xIovec.iov_len = sizeof( iobuf );

		xRet = FreeRTOS_TSN_recvmsg( xSocket, &xMsghdr, 0 );
		HANDLE_ERROR( xRet, xRet <= 0 );

		configPRINTF(( "Received %s\r\n", iobuf ));

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
	} while( 1 );

	do
	{
		xRet = FreeRTOS_TSN_recvfrom( xSocket, cMsg, sizeof( cMsg ), 0, &xSourceAddr, &uxSourceAddrLen );
		HANDLE_ERROR( xRet, xRet <= 0 );

		configPRINTF(( "Received: %s\r\n", cMsg ));

		if( strncmp( cMsg, "Followup", 8) == 0 )
		{
			configPRINTF(( "Received %s\r\n", cMsg ));
			sscanf( cMsg, "Followup %lu,%lu", &ts1.tv_sec, &ts1.tv_nsec );
		}
	}
	while( 1 );

	configPRINTF(( "Sending delay-req\r\n"));
	sprintf( cMsg, "Delayreq" );
	xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xMasterAddr, sizeof( xMasterAddr ) );
	HANDLE_ERROR( xRet, xRet <= 0 );

	do
	{
		xMsghdr.msg_namelen = sizeof( xSourceAddr );
		xMsghdr.msg_iovlen = 1;
		xMsghdr.msg_controllen = sizeof( cbuf );
		xIovec.iov_len = sizeof( iobuf );

		xRet = FreeRTOS_TSN_recvmsg( xSocket, &xMsghdr, FREERTOS_MSG_ERRQUEUE );
		HANDLE_ERROR( xRet, xRet <= 0 && xRet != -pdFREERTOS_ERRNO_EWOULDBLOCK );

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

	do
	{
		xRet = FreeRTOS_TSN_recvfrom( xSocket, cMsg, sizeof( cMsg ), 0, &xSourceAddr, &uxSourceAddrLen );
		HANDLE_ERROR( xRet, xRet <= 0 );

		configPRINTF(( "Received: %s\r\n", cMsg ));

		if( strncmp( cMsg, "Delayresp", 9) == 0 )
		{
			sscanf( cMsg, "Delayresp %lu,%lu", &ts4.tv_sec, &ts4.tv_nsec );
			break;
		}
	}
	while ( 1 );

	return;
}
