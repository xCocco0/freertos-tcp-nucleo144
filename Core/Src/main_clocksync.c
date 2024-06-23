
#include <string.h>

#include "main.h"

#include "FreeRTOS.h"

#include "FreeRTOS_IP.h"
#include "FreeRTOS_ARP.h"

#include "FreeRTOS_TSN_Sockets.h"
#include "FreeRTOS_TSN_Timestamp.h"
#include "FreeRTOS_TSN_Timebase.h"

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

/* Private function definitions */

static void vPing( struct freertos_sockaddr * const pxAddr )
{
	Socket_t xSocket = FreeRTOS_socket( FREERTOS_AF_INET4, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );

	struct freertos_sockaddr xSourceAddr, xDestinationAddr = *pxAddr;
	xSourceAddr.sin_port = FreeRTOS_htons( PORT_PING );
	xSourceAddr.sin_family = FREERTOS_AF_INET4;
	if( pxAddr->sin_addr == FreeRTOS_inet_addr( MASTER_IP ) )
	{
		xSourceAddr.sin_addr = FreeRTOS_inet_addr( SLAVE_IP );
	}
	else
	{
		xSourceAddr.sin_addr = FreeRTOS_inet_addr( MASTER_IP );
	}
	xDestinationAddr.sin_port = FreeRTOS_htons( PORT_PING );

	FreeRTOS_bind( xSocket, &xSourceAddr, sizeof( xSourceAddr ) );

	TickType_t xTimeout = pdMS_TO_TICKS( 10000 );
	FreeRTOS_setsockopt( xSocket, 0, FREERTOS_SO_SNDTIMEO, &xTimeout, sizeof( xTimeout ) );
	char cMsg[] = "hello";
	BaseType_t xRet = FreeRTOS_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xDestinationAddr, sizeof( xDestinationAddr ) );
	configPRINTF(("Ping request\r\n"));

	vTaskDelay(1000);
	xRet = FreeRTOS_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xDestinationAddr, sizeof( xDestinationAddr ) );
	vTaskDelay(1000);
	xRet = FreeRTOS_sendto( xSocket, cMsg, sizeof( cMsg ), 0, &xDestinationAddr, sizeof( xDestinationAddr ) );

	xRet = FreeRTOS_recvfrom( xSocket, cMsg, sizeof( cMsg ), 0, NULL, NULL );
	configPRINTF(("Ping received reply\r\n"));

	vTaskDelay( 1000 );
}

static BaseType_t prvSendAndRetrieveTimestamp( TSNSocket_t xSocket, char * cText, size_t uxTextSize, struct freertos_sockaddr * pxAddr, socklen_t uxAddrSize, struct msghdr * pxMsgh, struct freertos_timespec * pxTime )
{
	BaseType_t xRet;
	struct freertos_timespec * pxTimestamps;

	xRet = FreeRTOS_TSN_sendto( xSocket, cText, uxTextSize, 0, pxAddr, uxAddrSize );
	HANDLE_ERROR( xRet, xRet <= 0 );

	while( 1 )
	{
		xRet = FreeRTOS_TSN_recvmsg( xSocket, pxMsgh, FREERTOS_MSG_ERRQUEUE );
		HANDLE_ERROR( xRet, xRet < 0 && xRet != -pdFREERTOS_ERRNO_EWOULDBLOCK );

		configPRINTF(( "Received ancillary with ret %d\r\n", xRet ));

		if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
		{
			vTaskDelay( 500 );
			continue;
		}

		for( struct cmsghdr * cmsg = CMSG_FIRSTHDR( pxMsgh ); cmsg; cmsg = CMSG_NXTHDR( pxMsgh, cmsg ) )
		{
			if( cmsg->cmsg_level != FREERTOS_SOL_SOCKET )
				continue;

			switch( cmsg->cmsg_type )
			{
				case FREERTOS_SO_TIMESTAMPING:
					pxTimestamps = CMSG_DATA( cmsg );
					*pxTime = pxTimestamps[0];
					configPRINTF(( "Acquired ts: %lu, %lu\r\n", pxTime->tv_sec, pxTime->tv_nsec ));
					return 0;

				default:
					break;
			}
		}

		configPRINTF(( "Ancillary msg wrong\r\n" ));
	}

	return 0;
}

static BaseType_t prvRecvPrefixed( TSNSocket_t xSocket, char * cText, size_t uxTextSize, struct freertos_sockaddr * pxAddr, socklen_t * puxAddrSize, char * cPrefix, size_t uxPrefixSize )
{
	BaseType_t xRet;

	while( 1 )
	{
		xRet = FreeRTOS_TSN_recvfrom( xSocket, cText, uxTextSize, 0, pxAddr, puxAddrSize );
		HANDLE_ERROR( xRet, xRet < 0 && xRet != -pdFREERTOS_ERRNO_EWOULDBLOCK );

		if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
		{
			return xRet;
		}

		configPRINTF(( "Received: %s\r\n", cText ));

		if( strncmp( cText, cPrefix, uxPrefixSize ) == 0 )
		{
			configPRINTF(( "Token ok\r\n", cText ));
			break;
		}

		configPRINTF(( "Wrong token (expected: %s)\r\n", cPrefix ));
	}

	return 0;
}

static BaseType_t prvRecvPrefixedAndRetrieveTimestamp( TSNSocket_t xSocket, char * cText, size_t uxTextSize, struct msghdr * pxMsgh, char * cPrefix, size_t uxPrefixSize, struct freertos_timespec * pxTime )
{
	size_t uxControlBufSize = pxMsgh->msg_controllen, uxIovecSize = pxMsgh->msg_iov[0].iov_len;
	struct freertos_timespec * pxTimestamps;
	BaseType_t xRet;

	while( 1 )
	{
		pxMsgh->msg_namelen = sizeof( struct freertos_sockaddr );
		pxMsgh->msg_iovlen = 1;
		pxMsgh->msg_controllen = uxControlBufSize;
		pxMsgh->msg_iov[0].iov_len = uxIovecSize;

		xRet = FreeRTOS_TSN_recvmsg( xSocket, pxMsgh, 0 );
		HANDLE_ERROR( xRet, xRet < 0 && xRet != -pdFREERTOS_ERRNO_EWOULDBLOCK );

		if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
		{
			return xRet;
		}

		configPRINTF(("Received: %s\r\n", pxMsgh->msg_iov[0].iov_base ));

		if( pxMsgh->msg_iovlen > 0 && pxMsgh->msg_iov[0].iov_len > 0 && strncmp( pxMsgh->msg_iov[0].iov_base, cPrefix, uxPrefixSize ) == 0 )
		{
			configPRINTF(( "Token ok\r\n", pxMsgh->msg_iov[0].iov_base ));
			for( struct cmsghdr * cmsg = CMSG_FIRSTHDR( pxMsgh ); cmsg; cmsg = CMSG_NXTHDR( pxMsgh, cmsg ) )
			{
				if( cmsg->cmsg_level != FREERTOS_SOL_SOCKET )
					continue;

				switch( cmsg->cmsg_type )
				{
					case FREERTOS_SO_TIMESTAMPING:
						pxTimestamps = CMSG_DATA( cmsg );
						*pxTime = pxTimestamps[0];
						configPRINTF(( "Received ts: %lu, %lu\r\n", pxTime->tv_sec, pxTime->tv_nsec ));
						return 0;

					default:
						break;
				}
			}
			configPRINTF(( "Ancillary msg wrong\r\n" ));
		}

		configPRINTF(( "Wrong token (expected: %s)\r\n", cPrefix ));
	}

	return 0;
}


static void prvSyncTimebase( struct freertos_timespec * ts )
{
	configPRINTF(("T1: %3lu.%09lu s:\r\n", ts[1].tv_sec, ts[1].tv_nsec ));
	configPRINTF(("T2: %3lu.%09lu.s:\r\n", ts[2].tv_sec, ts[2].tv_sec ));
	configPRINTF(("T3: %3lu.%09lu s:\r\n", ts[3].tv_sec, ts[3].tv_nsec ));
	configPRINTF(("T4: %3lu.%09lu s:\r\n", ts[4].tv_sec, ts[4].tv_nsec ));

	/* (( t2 + t3 - t1 - t4 ) )/ 2 */
	/* underflow if t2 + t3 < t1 + t4 */
	struct freertos_timespec xTimeDiff, xVal1, xVal2, xCurrentTime;
	BaseType_t xPlus;

	xTimespecSum( &xVal1, &ts[2], &ts[3] );
	xTimespecSum( &xVal2, &ts[1], &ts[4] );

	configPRINTF(("val1: %3lu.%09lu s:\r\n", xVal1.tv_sec, xVal1.tv_nsec ));
	configPRINTF(("val2: %3lu.%09lu s:\r\n", xVal2.tv_sec, xVal2.tv_nsec ));

	configPRINTF(("Computed offset value:\r\n"));
	if( xTimespecCmp( &xVal1, &xVal2 ) == 1 )
	{
		/* Val1 > Val2 */
		xTimespecDiff( &xTimeDiff, &xVal1, &xVal2 );
		configPRINTF(("-\r\n"));
		xPlus = pdFALSE;
	}
	else
	{
		/* Val1 <= Val2 */
		xTimespecDiff( &xTimeDiff, &xVal2, &xVal1 );
		configPRINTF(("+\r\n"));
		xPlus = pdTRUE;
	}

	configPRINTF(("tmp: %3lu.%09lu s:\r\n", xTimeDiff.tv_sec, xTimeDiff.tv_nsec ));
	xTimespecDiv( &xTimeDiff, &xTimeDiff, 2 );

	configPRINTF(("%3lu.%09lu s:\r\n", xTimeDiff.tv_sec, xTimeDiff.tv_nsec ));

	/* Offset is computed, now adjust the timebase */

	portENTER_CRITICAL();
	vTimebaseGetTime( &xCurrentTime );
	if( xPlus == pdTRUE )
	{
		xTimespecSum( &xCurrentTime, &xCurrentTime, &xTimeDiff );
	}
	else
	{
		xTimespecDiff( &xCurrentTime, &xCurrentTime, &xTimeDiff );
	}
	vTimebaseSetTime( &xCurrentTime );
	portEXIT_CRITICAL();

}
/**
 * @brief  Task for the Master
 * @param  argument: Not used
 * @retval None
 */
void vTaskSyncMaster( void * pvArgument )
{
	configPRINTF(("Hey I am the master\r\n" ));

	TSNSocket_t xSocket = FreeRTOS_TSN_socket( FREERTOS_AF_INET4, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	HANDLE_ERROR( xSocket, xSocket == NULL || xSocket == FREERTOS_TSN_INVALID_SOCKET );

	BaseType_t xRet;
	uint32_t ulOpt = SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE;
	xRet = FreeRTOS_TSN_setsockopt( xSocket, FREERTOS_SOL_SOCKET, FREERTOS_SO_TIMESTAMPING, &ulOpt, sizeof( ulOpt ) );
	HANDLE_ERROR( xRet, xRet != 0 );

	TickType_t uxTimeout = pdMS_TO_TICKS( 1000 );
	xRet = FreeRTOS_TSN_setsockopt( xSocket, FREERTOS_SOL_SOCKET, FREERTOS_SO_RCVTIMEO, &uxTimeout, sizeof( uxTimeout ) );
	HANDLE_ERROR( xRet, xRet != 0 );
	
	char cMsg[ 32 ];

	struct freertos_sockaddr xSlaveAddr, xMasterAddr, xSourceAddr;
	xMasterAddr.sin_port = FreeRTOS_htons( PORT );
	xMasterAddr.sin_addr = FreeRTOS_inet_addr( MASTER_IP );
	xMasterAddr.sin_family = FREERTOS_AF_INET4;
	xSlaveAddr.sin_port = FreeRTOS_htons( PORT );
	xSlaveAddr.sin_addr = FreeRTOS_inet_addr( SLAVE_IP );
	xSlaveAddr.sin_family = FREERTOS_AF_INET4;
	socklen_t uxSourceAddrLen;

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
	struct freertos_timespec ts;
	
	do
	{
		while( 1 )
		{
			/* Sending sync */
			configPRINTF(( "Sending sync\r\n" ));

			sprintf( cMsg, "Sync");
			prvSendAndRetrieveTimestamp( xSocket, cMsg, strnlen( cMsg, sizeof( cMsg ) ) + 1, &xSlaveAddr, sizeof( xSlaveAddr ), &xMsghdr, &ts );

			/* Sending follow up */
			configPRINTF(( "Sending follow-up\r\n" ) );

			sprintf( cMsg, "Followup %lu,%lu", ts.tv_sec, ts.tv_nsec );
			xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, strnlen( cMsg, sizeof( cMsg ) ) + 1, 0, &xSlaveAddr, sizeof( xSlaveAddr ) );
			HANDLE_ERROR( xRet, xRet < 0 );

			/* Waiting for delay req */
			xRet = prvRecvPrefixedAndRetrieveTimestamp( xSocket, cMsg, sizeof( cMsg ), &xMsghdr, "Delayreq", sizeof("Delayreq")-1, &ts );
			if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
			{
				/* Starting back from sync */
				continue;
			}

			break;
		}

		while( 1 )
		{
			/* Sending delay resp */
			configPRINTF(( "Sending delay resp\r\n"));
			sprintf( cMsg, "Delayresp %lu,%lu", ts.tv_sec, ts.tv_nsec );
			xRet = FreeRTOS_TSN_sendto( xSocket, cMsg, strnlen( cMsg, sizeof( cMsg ) ) + 1, 0, &xSlaveAddr, sizeof( xSlaveAddr ) );
			HANDLE_ERROR( xRet, xRet < 0 );

			/* Receive term */
			xRet = prvRecvPrefixed( xSocket, cMsg, sizeof( cMsg ), &xSourceAddr, &uxSourceAddrLen, "Term", sizeof( "Term" )-1 );
			if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
			{
				/* Start back to delay resp */
				continue;
			}

			break;
		}

		/* Go back to start sync */
	}
	while( 1 );

	configPRINTF(("Task stopping\r\n"));
	vTaskDelete( NULL );
}

/**
 * @brief  Task for the Slave who will synchronize against the master's clock
 * @param  argument: Not used
 * @retval None
 */
void vTaskSyncSlave( void * pvArgument )
{
	configPRINTF(("Hello there, I am the slave\r\n" ));

	TSNSocket_t xSocket = FreeRTOS_TSN_socket( FREERTOS_AF_INET4, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	HANDLE_ERROR( xSocket, xSocket == NULL || xSocket == FREERTOS_TSN_INVALID_SOCKET );

	BaseType_t xRet;

	uint32_t ulOpt = SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE;
	xRet = FreeRTOS_TSN_setsockopt( xSocket, FREERTOS_SOL_SOCKET, FREERTOS_SO_TIMESTAMPING, &ulOpt, sizeof( ulOpt ) );
	HANDLE_ERROR( xRet, xRet != 0 );

	TickType_t uxTimeout = pdMS_TO_TICKS( 1000 );
	xRet = FreeRTOS_TSN_setsockopt( xSocket, FREERTOS_SOL_SOCKET, FREERTOS_SO_RCVTIMEO, &uxTimeout, sizeof( uxTimeout ) );
	HANDLE_ERROR( xRet, xRet != 0 );
	
	char cMsg[ 32 ];

	struct freertos_sockaddr xSlaveAddr, xMasterAddr, xSourceAddr;
	socklen_t uxSourceAddrLen;
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

	vPing( &xMasterAddr );

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
	struct freertos_timespec ts[4];
	
	while( 1 )
	{
		/* Waiting for sync */
		xRet = prvRecvPrefixedAndRetrieveTimestamp( xSocket, cMsg, sizeof( cMsg ), &xMsghdr, "Sync", sizeof("Sync")-1, &ts[2] );
		if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
		{
			/* Start back from beginning*/
			continue;
		}

		/* Receive follow-up */
		xRet = prvRecvPrefixed( xSocket, cMsg, sizeof( cMsg ), &xSourceAddr, &uxSourceAddrLen, "Followup", sizeof( "Followup" )-1 );
		if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
		{
			/* Start back from beginning*/
			continue;
		}
		sscanf( cMsg, "Followup %lu,%lu", &ts[1].tv_sec, &ts[1].tv_nsec );

		break;
	}

	do
	{
		while( 1 )
		{
			/* Send delay request */
			configPRINTF(( "Sending delay-req\r\n"));
			sprintf( cMsg, "Delayreq" );
			prvSendAndRetrieveTimestamp( xSocket, cMsg, strnlen( cMsg, sizeof( cMsg ) ) + 1, &xMasterAddr, sizeof( xMasterAddr ), &xMsghdr, &ts[3] );

			/* Receive delay resp */
			xRet = prvRecvPrefixed( xSocket, cMsg, sizeof( cMsg ), &xSourceAddr, &uxSourceAddrLen, "Delayresp", sizeof( "Delayresp" )-1 );
			if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
			{
				/* Start back from delay req */
				continue;
			}
			sscanf( cMsg, "Delayresp %lu,%lu", &ts[4].tv_sec, &ts[4].tv_nsec );

			break;
		}

		/* Handshake ended */

		prvSyncTimebase( ts );

		while( 1 )
		{
			/* Sending termination */
			configPRINTF(( "Sending term\r\n" ));

			sprintf( cMsg, "Term" );
			FreeRTOS_TSN_sendto( xSocket, cMsg, strnlen( cMsg, sizeof( cMsg ) ) + 1, 0, &xMasterAddr, sizeof( xMasterAddr ) );

			/* Waiting for sync */
			xRet = prvRecvPrefixedAndRetrieveTimestamp( xSocket, cMsg, sizeof( cMsg ), &xMsghdr, "Sync", sizeof("Sync")-1, &ts[2] );
			if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
			{
				/* Start back from term */
				continue;
			}

			/* Receive follow-up */
			xRet = prvRecvPrefixed( xSocket, cMsg, sizeof( cMsg ), &xSourceAddr, &uxSourceAddrLen, "Followup", sizeof( "Followup" )-1 );
			if( xRet == -pdFREERTOS_ERRNO_EWOULDBLOCK )
			{
				/* Start back from term */
				continue;
			}
			sscanf( cMsg, "Followup %lu,%lu", &ts[1].tv_sec, &ts[1].tv_nsec );

			break;
		}

		/* Go back to send delay req */

	} while( 1 );

	configPRINTF(("Task stopping\r\n"));
	vTaskDelete( NULL );
}

void vTaskLED( void * pvParameters )
{
	for( ; ; )
	{
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
		vTaskDelay( pdMS_TO_TICKS( 250 ) );
		HAL_GPIO_WritePin( LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET );
	}
}