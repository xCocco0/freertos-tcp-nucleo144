/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "peripherals.h"
#include "FreeRTOS_ARP.h"

#include "FreeRTOS_TSN_NetworkScheduler.h"
#include "FreeRTOS_TSN_Controller.h"
#include "FreeRTOS_TSN_VLANTags.h"
#include "FreeRTOS_TSN_Sockets.h"
#include "FreeRTOS_TSN_DS.h"
#include "FreeRTOS_TSN_Ancillary.h"
#include "FreeRTOS_TSN_Timestamp.h"
#include "NetworkWrapper.h"

#ifdef DEBUG
    #include "logger.h"
#endif

#define DST_ADDR_4    "169.254.174.41"
#define DST_ADDR_6    "fe80::4535:c3a4:9457:6e5a"
#define DST_PORT      10000

#define SRC_ADDR_4    "169.254.174.43"
#define SRC_ADDR_6    "fe80::dead:beef"
#define SRC_PORT      10000


void vTaskTSNTest( void * argument );
void vTaskTSNTestCBS( void * argument );
void vTaskUDPSendIPv4( void * argument );
void vTaskUDPSendIPv6( void * argument );
void vTaskTCPSendIPv4( void * argument );
void vTaskTCPSendIPv6( void * argument );

const uint8_t xUDP_IPv4_test[] =
{
    configMAC_ADDR0,
    configMAC_ADDR1,
    configMAC_ADDR2,
    configMAC_ADDR3,
    configMAC_ADDR4,
    configMAC_ADDR5,/*dst mac */
    configMAC_ADDR0,
    configMAC_ADDR1,
    configMAC_ADDR2,
    configMAC_ADDR3,
    configMAC_ADDR4,
    configMAC_ADDR5,        /*src mac */
    0x08,           0x00,   /*frame type */
    0x45,                   /*ip header len */
    0x0,                    /*ip dscp */
    0x0,            20 + 5, /*ip len */
    0x0,            0x0,    /*ip id */
    0x0,            0x0,    /*ip frag offset */
    0xff,                   /*ip ttl */
    0x11,                   /*ip proto */
    0x0,            0x0,    /*ip chksum */
    configIP_ADDR0,
    configIP_ADDR1,
    configIP_ADDR2,
    configIP_ADDR3, /*src ip */
    configIP_ADDR0,
    configIP_ADDR1,
    configIP_ADDR2,
    configIP_ADDR3,              /*dst ip */
    0x27,           0x10,        /*src port */
    0x27,           0x10,        /*dst port */
    0x0,            5,           /*udp length */
    0x0,            0x0,         /*udp chksum */
    0x61,           0x61,0x61, 0x61, 0x61 /*udp payload */
};

void prvFakeReceiveFrame( const uint8_t * pxBuf,
                          size_t uxSize )
{
    NetworkBufferDescriptor_t * pxNetworkBuffer;

    pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( uxSize, portMAX_DELAY );
    memcpy( pxNetworkBuffer->pucEthernetBuffer, pxBuf, uxSize );

    pxNetworkBuffer->pxInterface = FreeRTOS_FirstNetworkInterface();
    pxNetworkBuffer->pxEndPoint = FreeRTOS_MatchingEndpoint( pxNetworkBuffer->pxInterface, pxNetworkBuffer->pucEthernetBuffer );
    pxNetworkBuffer->xDataLength = uxSize;

    IPStackEvent_t xRxEvent;

    xRxEvent.eEventType = eNetworkRxEvent;
    xRxEvent.pvData = ( void * ) pxNetworkBuffer;

    ( void ) xSendEventStructToTSNController( &xRxEvent, portMAX_DELAY );
}

/**
 * @brief  Test
 * @param  argument: Not used
 * @retval None
 */
void vTaskTSNTest( void * argvument )
{
    BaseType_t xRet;
    BaseType_t xSockOpt;

    struct freertos_timespec * ts;

    /* -- create socket -- */
    configPRINTF( ( "Creating socket...\r\n" ) );
    TSNSocket_t xSock = FreeRTOS_TSN_socket( FREERTOS_AF_INET4,
                                             FREERTOS_SOCK_DGRAM,
                                             FREERTOS_IPPROTO_UDP );

    if( xSock == FREERTOS_TSN_INVALID_SOCKET )
    {
        configPRINTF( ( "Socket error\r\n" ) );

        for( ; ; )
        {
        }
    }

    configPRINTF( ( "Done!\r\n" ) );

    #if ( tsnconfigSOCKET_INSERTS_VLAN_TAGS != tsnconfigDISABLE )
        xSockOpt = vlantagCLASS_1;

        if( FreeRTOS_TSN_setsockopt( xSock, 0, FREERTOS_SO_VLAN_CTAG_PCP, &xSockOpt, sizeof( xSockOpt ) ) != 0 )
        {
            configPRINTF( ( "setsockopt error\r\n" ) );

            for( ; ; )
            {
            }
        }

        xSockOpt = vlantagCLASS_2;

        if( FreeRTOS_TSN_setsockopt( xSock, 0, FREERTOS_SO_VLAN_STAG_PCP, &xSockOpt, sizeof( xSockOpt ) ) != 0 )
        {
            configPRINTF( ( "setsockopt error\r\n" ) );

            for( ; ; )
            {
            }
        }
    #endif /* if ( tsnconfigSOCKET_INSERTS_VLAN_TAGS != tsnconfigDISABLE ) */
    xSockOpt = diffservCLASS_AFxy( 2, 3 );

    if( FreeRTOS_TSN_setsockopt( xSock, 0, FREERTOS_SO_DS_CLASS, &xSockOpt, sizeof( xSockOpt ) ) != 0 )
    {
        configPRINTF( ( "setsockopt error\r\n" ) );

        for( ; ; )
        {
        }
    }

    xSockOpt = SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE;

    if( FreeRTOS_TSN_setsockopt( xSock, FREERTOS_SOL_SOCKET, FREERTOS_SO_TIMESTAMPING, &xSockOpt, sizeof( xSockOpt ) ) != 0 )
    {
        configPRINTF( ( "setsockopt error\r\n" ) );

        for( ; ; )
        {
        }
    }

    UBaseType_t xSourceAddrLen;
    struct freertos_sockaddr xDestinationAddress, xSourceAddress;
    xDestinationAddress.sin_addr = FreeRTOS_inet_addr( DST_ADDR_4 );
    xDestinationAddress.sin_port = FreeRTOS_htons( DST_PORT );
    xDestinationAddress.sin_family = FREERTOS_AF_INET;
    xSourceAddress.sin_addr = FreeRTOS_inet_addr( SRC_ADDR_4 );
    xSourceAddress.sin_port = FreeRTOS_htons( SRC_PORT );
    xSourceAddress.sin_family = FREERTOS_AF_INET;

    /* -- bind socket -- */
    configPRINTF( ( "Binding socket...\r\n" ) );
    xRet = FreeRTOS_TSN_bind( xSock, &xSourceAddress, sizeof( xSourceAddress ) );

    if( xRet )
    {
        configPRINTF( ( "Socket could not be bound: error %d\r\n", xRet ) );

        for( ; ; )
        {
        }
    }

    configPRINTF( ( "Done!\r\n" ) );

    MACAddress_t xDestMAC = { .ucBytes = { 0x8C, 0xDC, 0xD4, 0x90, 0x5D, 0x33 } };
    NetworkEndPoint_t * pxEndPoint = FreeRTOS_FindEndPointOnNetMask( xDestinationAddress.sin_address.ulIP_IPv4, 14 );

    vTaskDelay( 1000U );
    vARPRefreshCacheEntry( &xDestMAC, xDestinationAddress.sin_address.ulIP_IPv4, pxEndPoint );

    char cMsg[ 64 ];
    char iobuf[ 128 ];
    union
    {
        struct msghdr alignment;
        char data[ 128 ];
    }
    cbuf;
    struct iovec xIovec =
    {
        .iov_base = iobuf,
        .iov_len  = sizeof( iobuf )
    };
    struct msghdr xMsghdr =
    {
        .msg_name       = &xSourceAddress,
        .msg_namelen    = sizeof( xSourceAddress ),
        .msg_iov        = &xIovec,
        .msg_iovlen     = 1,
        .msg_control    = &cbuf.data,
        .msg_controllen = sizeof( cbuf ),
    };

    void * dummy; /* for debugging */

    sprintf( cMsg, "Hello world n.%d", 0 );
    configPRINTF( ( "Sending message \"%s\"...", cMsg ) );

    xRet = FreeRTOS_TSN_sendto( xSock, cMsg, configMIN( sizeof( cMsg ), strlen( cMsg ) ), 0,
                                &xDestinationAddress, sizeof( xDestinationAddress ) );
    configPRINTF( ( "sent %d bytes.\r\n", xRet ) );

    if( xRet < 0 )
    {
        configPRINTF( ( "sendto error %d.\r\n", xRet ) );

        for( ; ; )
        {
        }
    }

    prvFakeReceiveFrame( xUDP_IPv4_test, sizeof( xUDP_IPv4_test ) );

    for( int i = 0; ; ++i )
    {
        configPRINTF( ( "Receiving...\r\n" ) );
        xRet = FreeRTOS_TSN_recvmsg( xSock, &xMsghdr, 0 /*FREERTOS_MSG_ERRQUEUE*/ );

        if( xRet < 0 )
        {
            configPRINTF( ( "recvfrom error %d.\r\n", xRet ) );

            for( ; ; )
            {
            }
        }

        configPRINTF( ( "Received %d bytes\r\n", xRet ) );

        for( struct cmsghdr * cmsg = CMSG_FIRSTHDR( &xMsghdr ); cmsg; cmsg = CMSG_NXTHDR( &xMsghdr, cmsg ) )
        {
            /* if( cmsg->cmsg_level != FREERTOS_SOL_SOCKET ) */
            /*  continue; */

            switch( cmsg->cmsg_type )
            {
                case FREERTOS_SO_TIMESTAMPNS:
                    ts = ( struct freertos_timespec * ) CMSG_DATA( cmsg );
                    break;

                case FREERTOS_SO_TIMESTAMPING:
                    ts = ( struct freertos_timespec * ) CMSG_DATA( cmsg );
                    break;

                default:
                    /* Ignore other cmsg options */
                    dummy = CMSG_DATA( cmsg );
                    ( void ) dummy;
                    break;
            }
        }

        vTaskDelay( pdMS_TO_TICKS( 2000 ) );
    }
}

/**
 * @brief  Test cbs
 * @param  argument: Not used
 * @retval None
 */
void vTaskTSNTestCBS( void * argvument )
{
    BaseType_t xRet;
    BaseType_t xSockOpt;

    struct freertos_timespec * ts;

    /* -- create socket -- */
    configPRINTF( ( "Creating socket...\r\n" ) );
    TSNSocket_t xSock = FreeRTOS_TSN_socket( FREERTOS_AF_INET4,
                                             FREERTOS_SOCK_DGRAM,
                                             FREERTOS_IPPROTO_UDP );

    if( xSock == FREERTOS_TSN_INVALID_SOCKET )
    {
        configPRINTF( ( "Socket error\r\n" ) );

        for( ; ; )
        {
        }
    }

    configPRINTF( ( "Done!\r\n" ) );

    xSockOpt = diffservCLASS_AFxy( 2, 3 );

    if( FreeRTOS_TSN_setsockopt( xSock, 0, FREERTOS_SO_DS_CLASS, &xSockOpt, sizeof( xSockOpt ) ) != 0 )
    {
        configPRINTF( ( "setsockopt error\r\n" ) );

        for( ; ; )
        {
        }
    }

    struct freertos_sockaddr xDestinationAddress, xSourceAddress;
    xDestinationAddress.sin_addr = FreeRTOS_inet_addr( DST_ADDR_4 );
    xDestinationAddress.sin_port = FreeRTOS_htons( 10003 );
    xDestinationAddress.sin_family = FREERTOS_AF_INET;
    xSourceAddress.sin_addr = FreeRTOS_inet_addr( SRC_ADDR_4 );
    xSourceAddress.sin_port = FreeRTOS_htons( 10003 );
    xSourceAddress.sin_family = FREERTOS_AF_INET;

    /* -- bind socket -- */
    configPRINTF( ( "Binding socket...\r\n" ) );
    xRet = FreeRTOS_TSN_bind( xSock, &xSourceAddress, sizeof( xSourceAddress ) );

    if( xRet )
    {
        configPRINTF( ( "Socket could not be bound: error %d\r\n", xRet ) );

        for( ; ; )
        {
        }
    }

    configPRINTF( ( "Done!\r\n" ) );

    NetworkEndPoint_t * pxEndPoint = FreeRTOS_FindEndPointOnNetMask( xDestinationAddress.sin_address.ulIP_IPv4, 10 );
    vARPRefreshCacheEntry( NULL, xDestinationAddress.sin_address.ulIP_IPv4, pxEndPoint );

    while( !xIsIPInARPCache( xDestinationAddress.sin_addr ) )
    {
        FreeRTOS_OutputARPRequest( xDestinationAddress.sin_addr );
		configPRINTF(("Sending ARP request to %xip\r\n", xDestinationAddress.sin_addr ));
        vTaskDelay( 1000U );
    }

	char cMsg[ 32 ];

	for( int i = 0; i < 10; ++i )
	{
		sprintf( cMsg, "Hello world n.%d", i );
		configPRINTF( ( "Sending message \"%s\"...", cMsg ) );

		xRet = FreeRTOS_TSN_sendto( xSock, cMsg, configMIN( sizeof( cMsg ), strlen( cMsg ) ), 0,
									&xDestinationAddress, sizeof( xDestinationAddress ) );
		configPRINTF( ( "sent %d bytes.\r\n", xRet ) );

		if( xRet < 0 )
		{
			configPRINTF( ( "sendto error %d.\r\n", xRet ) );

			for( ; ; )
			{
			}
		}
	}

	for(;;);
}

/**
 * @brief  Periodically send UDP packet using IPv4
 * @param  argument: Not used
 * @retval None
 */
void vTaskUDPSendIPv4( void * argument )
{
    BaseType_t xRet;

    configPRINTF( ( "vTaskUDPSendIPv4 started\r\n" ) );

    for( ; ; )
    {
        /* -- create socket -- */
        configPRINTF( ( "Creating socket...\r\n" ) );
        Socket_t xSock = FreeRTOS_socket( FREERTOS_AF_INET4,
                                          FREERTOS_SOCK_DGRAM,
                                          FREERTOS_IPPROTO_UDP );

        if( xSock == FREERTOS_INVALID_SOCKET )
        {
            configPRINTF( ( "Socket error\r\n" ) );

            for( ; ; )
            {
            }
        }

        configPRINTF( ( "Done!\r\n" ) );

        /* -- bind socket -- */
        socklen_t xSize = sizeof( struct freertos_sockaddr );
        configPRINTF( ( "Binding socket...\r\n" ) );
        xRet = FreeRTOS_bind( xSock, NULL, xSize );

        if( xRet )
        {
            configPRINTF( ( "Socket could not be bound: error %d\r\n", xRet ) );

            for( ; ; )
            {
            }
        }

        configPRINTF( ( "Done!\r\n" ) );

        struct freertos_sockaddr xDestinationAddress;
        xDestinationAddress.sin_addr = FreeRTOS_inet_addr( DST_ADDR_4 );
        xDestinationAddress.sin_port = FreeRTOS_htons( DST_PORT );
        xDestinationAddress.sin_family = FREERTOS_AF_INET;

        char cMsg[ 32 ];

        for( int i = 0; ; ++i )
        {
            sprintf( cMsg, "Hello world n.%d", i );
            configPRINTF( ( "Sending message \"%s\"...", cMsg ) );

            xRet = FreeRTOS_sendto( xSock, cMsg, configMIN( sizeof( cMsg ), strlen( cMsg ) ), 0,
                                    &xDestinationAddress, sizeof( xDestinationAddress ) );
            configPRINTF( ( "sent %d bytes.\r\n", xRet ) );
            configASSERT( xRet >= 0 );
            HAL_GPIO_TogglePin( GPIOB, LD3_Pin );
            vTaskDelay( 5000UL );
            /*vTaskDelay(1000UL / portTICK_PERIOD_MS); */
        }
    }
}

/**
 * @brief  Periodically send UDP packet using IPv6
 * @param  argument: Not used
 * @retval None
 */
void vTaskUDPSendIPv6( void * argument )
{
    BaseType_t xRet;

    configPRINTF( ( "vTaskUDPSendIPv6 started\r\n" ) );

    for( ; ; )
    {
        /* -- create socket -- */
        configPRINTF( ( "Creating socket...\r\n" ) );
        Socket_t xSock = FreeRTOS_socket( FREERTOS_AF_INET6,
                                          FREERTOS_SOCK_DGRAM,
                                          FREERTOS_IPPROTO_UDP );

        if( xSock == FREERTOS_INVALID_SOCKET )
        {
            configPRINTF( ( "Socket error\r\n" ) );

            for( ; ; )
            {
            }
        }

        configPRINTF( ( "Done!\r\n" ) );

        /* -- bind socket -- */
        socklen_t xSize = sizeof( struct freertos_sockaddr );
        configPRINTF( ( "Binding socket...\r\n" ) );
        xRet = FreeRTOS_bind( xSock, NULL, xSize );

        if( xRet )
        {
            configPRINTF( ( "Socket could not be bound: error %d\r\n", xRet ) );

            for( ; ; )
            {
            }
        }

        configPRINTF( ( "Done!\r\n" ) );

        struct freertos_sockaddr xDestinationAddress;
        FreeRTOS_inet_pton6( DST_ADDR_6, &xDestinationAddress.sin_address.xIP_IPv6.ucBytes );
        xDestinationAddress.sin_port = FreeRTOS_htons( DST_PORT );
        /*xDestinationAddress.sin_family = FREERTOS_EF_INET6; */

        char cMsg[ 32 ];

        for( int i = 0; ; ++i )
        {
            sprintf( cMsg, "Hello world n.%d", i );
            configPRINTF( ( "Sending message \"%s\"...", cMsg ) );

            xRet = FreeRTOS_sendto( xSock, cMsg, configMIN( sizeof( cMsg ), strlen( cMsg ) ), 0,
                                    &xDestinationAddress, sizeof( xDestinationAddress ) );
            configPRINTF( ( "sent %d bytes.\r\n", xRet ) );
            configASSERT( xRet >= 0 );
            HAL_GPIO_TogglePin( GPIOB, LD3_Pin );
            vTaskDelay( 5000UL );
            /*vTaskDelay(1000UL / portTICK_PERIOD_MS); */
        }
    }
}


/**
 * @brief  Periodically send TCP packets using IPv4
 * @param  argument: Not used
 * @retval None
 */
void vTaskTCPSendIPv4( void * argument )
{
    BaseType_t xRet;

    configPRINTF( ( "vTaskTCPSendIPv4 started\r\n" ) );

    for( ; ; )
    {
        /* -- create socket -- */
        configPRINTF( ( "Creating socket...\r\n" ) );
        Socket_t xSock = FreeRTOS_socket( FREERTOS_AF_INET4,
                                          FREERTOS_SOCK_STREAM,
                                          FREERTOS_IPPROTO_TCP );

        if( xSock == FREERTOS_INVALID_SOCKET )
        {
            HAL_GPIO_TogglePin( GPIOB, LD2_Pin );
            configPRINTF( ( "Socket error\r\n" ) );

            for( ; ; )
            {
            }
        }

        configPRINTF( ( "Done!\r\n" ) );

        /* -- bind socket -- */
        struct freertos_sockaddr xSourceAddress;
        xSourceAddress.sin_addr = FreeRTOS_inet_addr( SRC_ADDR_4 );
        xSourceAddress.sin_port = FreeRTOS_htons( SRC_PORT );
        xSourceAddress.sin_family = FREERTOS_AF_INET4;

        socklen_t xSize = sizeof( struct freertos_sockaddr );
        configPRINTF( ( "Binding socket...\r\n" ) );
        xRet = FreeRTOS_bind( xSock, &xSourceAddress, xSize );

        if( xRet )
        {
            configPRINTF( ( "Socket could not be bound: error %d\r\n", xRet ) );

            for( ; ; )
            {
            }
        }

        configPRINTF( ( "Done!\r\n" ) );

        struct freertos_sockaddr xDestinationAddress;
        xDestinationAddress.sin_addr = FreeRTOS_inet_addr( DST_ADDR_4 );
        xDestinationAddress.sin_port = FreeRTOS_htons( DST_PORT );
        xDestinationAddress.sin_family = FREERTOS_AF_INET4; /*FREERTOS_AFxy_INET(4,3); */

        /* -- connect socket -- */
        configPRINTF( ( "Connecting socket...\r\n" ) );
        xRet = FreeRTOS_connect( xSock, &xDestinationAddress, xSize );

        if( xRet )
        {
            configPRINTF( ( "Socket could not be connected: error %d\r\n", xRet ) );

            for( ; ; )
            {
            }
        }

        configPRINTF( ( "Done!\r\n" ) );

        char cMsg[ 32 ];

        for( int i = 0; ; ++i )
        {
            sprintf( cMsg, "Hello world n.%d", i );
            configPRINTF( ( "Sending message \"%s\"...", cMsg ) );

            xRet = FreeRTOS_send( xSock, cMsg, configMIN( sizeof( cMsg ), strlen( cMsg ) ), 0 );

            configPRINTF( ( "sent %d bytes.\r\n", xRet ) );
            configASSERT( xRet >= 0 );
            HAL_GPIO_TogglePin( GPIOB, LD3_Pin );
            vTaskDelay( 5000UL );
            /*vTaskDelay(1000UL / portTICK_PERIOD_MS); */
        }
    }
}

#if 0
    void task_( void * argument )
    {
        BaseType_t xRet;

        configPRINTF( ( "Running StartDefaultTask function\r\n" ) );

        for( ; ; )
        {
            configPRINTF( ( "Creating socket...\r\n" ) );
            Socket_t xSock = FreeRTOS_socket( FREERTOS_AF_INET4,
                                              /*FREERTOS_SOCK_STREAM, */
                                              /*FREERTOS_IPPROTO_TCP); */
                                              FREERTOS_SOCK_DGRAM,
                                              FREERTOS_IPPROTO_UDP );

            if( xSock == FREERTOS_INVALID_SOCKET )
            {
                HAL_GPIO_TogglePin( GPIOB, LD2_Pin );
                configPRINTF( ( "Socket error\r\n" ) );

                for( ; ; )
                {
                }
            }

            configPRINTF( ( "Done!\r\n" ) );

            struct freertos_sockaddr xSourceAddress;
            /*xSourceAddress.sin_addr = FreeRTOS_inet_addr_quick( ucIPAddress[ 0 ], ucIPAddress[ 1 ], ucIPAddress[ 2 ], ucIPAddress[ 3 ] ); */
            xSourceAddress.sin_port = FreeRTOS_htons( SRC_PORT );
            xSourceAddress.sin_family = FREERTOS_AF_INET4;

            socklen_t xSize = sizeof( struct freertos_sockaddr );
            configPRINTF( ( "Binding socket...\r\n" ) );
            xRet = FreeRTOS_bind( xSock, /*&xSourceAddress*/ NULL, xSize );

            if( xRet )
            {
                configPRINTF( ( "Socket could not be bound: error %d\r\n", xRet ) );

                for( ; ; )
                {
                }
            }

            configPRINTF( ( "Done!\r\n" ) );

            struct freertos_sockaddr xDestinationAddress;
            xDestinationAddress.sin_addr = FreeRTOS_inet_addr( DST_ADDR_4 );
            /*xDestinationAddress.sin_addr = FreeRTOS_inet_addr("192.168.56.1"); */
            /*xDestinationAddress.sin_addr = FreeRTOS_inet_addr("255.255.255.255"); */
            xDestinationAddress.sin_port = FreeRTOS_htons( DST_PORT );
            xDestinationAddress.sin_family = FREERTOS_AFxy_INET( 4, 3 );

            /*
             * configPRINTF( ("Connecting socket...\r\n") );
             * xRet = FreeRTOS_connect(xSock, &xDestinationAddress, xSize);
             * if(xRet) {
             * configPRINTF( ("Socket could not be connected: error %d\r\n", xRet) );
             * for(;;);
             * }
             * configPRINTF( ("Done!\r\n") );
             */
            char cMsg[ 32 ];

            for( int i = 0; ; ++i )
            {
                sprintf( cMsg, "Hello world n.%d", i );
                configPRINTF( ( "Sending message \"%s\"...", cMsg ) );

                xRet = FreeRTOS_sendto( xSock, cMsg, configMIN( sizeof( cMsg ), strlen( cMsg ) ), 0,
                                        &xDestinationAddress, sizeof( xDestinationAddress )
                                        );

                /*
                 * xRet = FreeRTOS_send(xSock, cMsg, configMIN(sizeof(cMsg), strlen(cMsg)), 0);
                 */
                configPRINTF( ( "sent %d bytes.\r\n", xRet ) );
                configASSERT( xRet >= 0 );
                HAL_GPIO_TogglePin( GPIOB, LD3_Pin );
                vTaskDelay( 1000UL );
                /*vTaskDelay(1000UL / portTICK_PERIOD_MS); */
            }
        }
    }
#endif /* if 0 */
