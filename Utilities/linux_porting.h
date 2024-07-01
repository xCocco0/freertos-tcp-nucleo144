
/* This helps porting a source for this project to Linux.
 * This is incomplete and currently only supports main_clocksync.c
 */

#ifndef LINUX_PORTING_H
#define LINUX_PORTING_H

#ifndef __gnu_linux__
	#error Must be compiled for Linux!
#endif

#define _GNU_SOURCE
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/net_tstamp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <linux/errqueue.h>

#define configPRINTF(x) printf x

#define TSNSocket_t int
#define FreeRTOS_TSN_socket socket
#define FreeRTOS_TSN_bind bind
#define FreeRTOS_TSN_sendto sendto
#define FreeRTOS_TSN_recvfrom recvfrom
#define FreeRTOS_TSN_recvmsg recvmsg
#define FreeRTOS_TSN_setsockopt setsockopt

#define NetworkEndPoint_t int
#define FreeRTOS_FindEndPointOnNetMask( x, y ) NULL
#define vARPRefreshCacheEntry( x, y, z )
#define xIsIPInARPCache( x ) 1
#define FreeRTOS_OutputARPRequest( x )

#define BaseType_t int32_t
#define freertos_sockaddr sockaddr_in
#define TickType_t struct timeval
#define freertos_timespec timespec
#define FreeRTOS_htons htons
#define FreeRTOS_inet_addr_quick( ucOctet0, ucOctet1, ucOctet2, ucOctet3 ) \
	( ( ( ( uint32_t ) ( ucOctet3 ) ) << 24 ) |                            \
	  ( ( ( uint32_t ) ( ucOctet2 ) ) << 16 ) |                            \
	  ( ( ( uint32_t ) ( ucOctet1 ) ) << 8 ) |                             \
	  ( ( uint32_t ) ( ucOctet0 ) ) )
#define sin_addr sin_addr.s_addr

#define GET_TIMEOUT_MS( x ) { .tv_sec = ( x / 1000 ), .tv_usec = ( x % 1000 ) * 1000 }

#define FREERTOS_TSN_INVALID_SOCKET NULL
#define FREERTOS_AF_INET4 AF_INET
#define FREERTOS_SOCK_DGRAM SOCK_DGRAM
#define FREERTOS_IPPROTO_UDP IPPROTO_UDP
#define FREERTOS_SO_RCVTIMEO SO_RCVTIMEO
#define FREERTOS_MSG_ERRQUEUE MSG_ERRQUEUE
#define FREERTOS_SOL_SOCKET SOL_SOCKET
#define FREERTOS_SO_TIMESTAMPING SO_TIMESTAMPING
#define pdFREERTOS_ERRNO_EWOULDBLOCK 1
#define pdFREERTOS_ERRNO_EAGAIN 1

#define pdMS_TO_TICKS( x ) x
#define pdTRUE 1
#define pdFALSE 0

/* Slave time computation not supported yet
 */
#define xTimespecSum( x, y, z ) 1
#define xTimespecDiff( x, y, z ) 1
#define xTimespecCmp( x, y ) 1
#define xTimespecDiv( x, y, z ) 1
#define vTimebaseSetTime( x )
#define vTimebaseGetTime( x )
#define vTimebaseAdjTime( x, y )

#define vTaskSuspendAll()
#define xTaskResumeAll() 0
#define vTaskDelete( x )
#define vLoggerPrintFromISR(...) printf(__VA_ARGS__)
#define vTaskDelay( x ) usleep( 1000 * x )

#define __MAIN_H
#define INC_FREERTOS_H
#define FREERTOS_IP_H
#define FREERTOS_ARP_H
#define FREERTOS_TSN_SOCKETS_H
#define FREERTOS_TSN_TIMESTAMP_H
#define FREERTOS_TSN_TIMEBASE_H

#endif /* LINUX_PORTING_H */
