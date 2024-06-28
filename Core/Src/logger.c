/**
 * @file logger.c
 * @brief This file contains the implementation of a logger module for printing log messages.
 *
 * The logger module provides functions for printing log messages to a buffer and periodically
 * printing the buffer contents to a UART peripheral. It uses FreeRTOS tasks and semaphores for
 * synchronization.
 */

#include "logger.h"
#include "peripherals.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include <string.h>
#include <stdarg.h>
#include "printf_stdarg.h"

#define LOGGER_BUF_LEN           4096U
#define LOGGER_TASK_PRIORITY     tskIDLE_PRIORITY + 2
#define LOGGER_STACK_SIZE        LOGGER_BUF_LEN / 4 + 16

#define PRINTER_TICKS_TO_WAIT    ( TickType_t ) 10U

static struct
{
    SemaphoreHandle_t xLock;
    char pcBuffer[ LOGGER_BUF_LEN ];
    char pcCopyBuffer[ LOGGER_BUF_LEN ];

    uint16_t usWriteIndex;
    uint16_t usReadIndex;

    uint8_t ucTimedOut;
}
xLoggerData;

TaskHandle_t xLoggerTaskHandle = NULL;
StaticTask_t xLoggerTCB;
StackType_t xLoggerStack[ LOGGER_STACK_SIZE ];

StaticSemaphore_t xLoggerSemaphoreBuffer;

/// @brief Update the logger buffer with the contents of the copy buffer.
/// @param usRequiredLen  The number of bytes to copy from the copy buffer to the logger buffer.
void prvUpdateFromCopyBuffer( uint16_t usRequiredLen );

/// @brief Print a string to the UART peripheral.
/// @param pcStr 
/// @param usLen 
void prvPutString( char * const pcStr,
                   uint32_t usLen );

/*
 * This function will periodically check
 * the Logger buffer and print the values
 */
void prvLoggerTask( void * const pvParameters );

/**** public functions ****/

/// @brief Initialize the logger module.
/// @param  
void vLoggerInit( void )
{
    xLoggerData.xLock = xSemaphoreCreateMutexStatic( &xLoggerSemaphoreBuffer );
    xLoggerData.usWriteIndex = 0;
    xLoggerData.usReadIndex = 0;

    xLoggerTaskHandle = xTaskCreateStatic( prvLoggerTask,
                                           "Logger task",
                                           LOGGER_STACK_SIZE,
                                           NULL,
                                           LOGGER_TASK_PRIORITY,
                                           xLoggerStack,
                                           &xLoggerTCB
                                           );
}

/// @brief Print a log message to the logger buffer.
/// @param format 
/// @param  
void vLoggerPrintFromISR( const char * format,
                          ... )
{
    va_list args;
    uint16_t usRequiredLen, sMaxLen;
    BaseType_t xHigherPriorityTaskWoken;

    va_start( args, format );

    if( xSemaphoreTakeFromISR( xLoggerData.xLock, &xHigherPriorityTaskWoken ) != pdFALSE )
    {
        sMaxLen = xLoggerData.usReadIndex - xLoggerData.usWriteIndex;

        if( sMaxLen <= 0 )
        {
            sMaxLen += LOGGER_BUF_LEN;
        }

        usRequiredLen = vsnprintf(
            xLoggerData.pcCopyBuffer,
            LOGGER_BUF_LEN,
            format,
            args );

        usRequiredLen = configMIN( usRequiredLen, sMaxLen );

        ( void ) prvUpdateFromCopyBuffer( usRequiredLen );

        vTaskNotifyGiveFromISR( xLoggerTaskHandle, &xHigherPriorityTaskWoken );
        xSemaphoreGiveFromISR( xLoggerData.xLock, &xHigherPriorityTaskWoken );
    }
    else
    {
        xLoggerData.ucTimedOut = 1;
    }

    va_end( args );

    /* xHigherPriorityTaskWoken currently unused */
}

/// @brief Print a log message to the logger buffer.
/// @param format 
/// @param  
void vLoggerPrint( const char * format,
                   ... )
{
    va_list args;
    uint16_t usRequiredLen, sMaxLen;

    va_start( args, format );

    if( xSemaphoreTake( xLoggerData.xLock, PRINTER_TICKS_TO_WAIT ) != pdFALSE )
    {
        sMaxLen = xLoggerData.usReadIndex - xLoggerData.usWriteIndex;

        if( sMaxLen <= 0 )
        {
            sMaxLen += LOGGER_BUF_LEN;
        }

        usRequiredLen = vsnprintf(
            xLoggerData.pcCopyBuffer,
            LOGGER_BUF_LEN,
            format,
            args );

        usRequiredLen = configMIN( usRequiredLen, sMaxLen );

        ( void ) prvUpdateFromCopyBuffer( usRequiredLen );

        xTaskNotifyGive( xLoggerTaskHandle );
        xSemaphoreGive( xLoggerData.xLock );
    }
    else
    {
        xLoggerData.ucTimedOut = 1;
    }

    va_end( args );
}

/// @brief Print a log message to the logger buffer and append a newline character.
/// @param format 
/// @param  
void vLoggerPrintline( const char * format,
                       ... )
{
    va_list args;
    uint16_t usRequiredLen, sMaxLen;

    va_start( args, format );

    if( xSemaphoreTake( xLoggerData.xLock, PRINTER_TICKS_TO_WAIT ) != pdFALSE )
    {
        sMaxLen = xLoggerData.usReadIndex - xLoggerData.usWriteIndex;

        if( sMaxLen <= 0 )
        {
            sMaxLen += LOGGER_BUF_LEN;
        }

        usRequiredLen = vsnprintf(
            xLoggerData.pcCopyBuffer,
            LOGGER_BUF_LEN,
            format,
            args );

        usRequiredLen = configMIN( usRequiredLen, sMaxLen - 2 );

        char cChar;

        while( usRequiredLen > 0 )
        {
            cChar = xLoggerData.pcCopyBuffer[ usRequiredLen - 1 ];

            if( ( cChar == '\r' ) || ( cChar == '\n' ) )
            {
                usRequiredLen -= 1;
            }
            else
            {
                break;
            }
        }

        xLoggerData.pcCopyBuffer[ usRequiredLen ] = '\r';
        xLoggerData.pcCopyBuffer[ usRequiredLen + 1 ] = '\n';
        usRequiredLen += 2;

        ( void ) prvUpdateFromCopyBuffer( usRequiredLen );

        xTaskNotifyGive( xLoggerTaskHandle );
        xSemaphoreGive( xLoggerData.xLock );
    }

    va_end( args );
}

/**** private functions ****/

/// @brief Update the logger buffer with the contents of the copy buffer.
/// @param usRequiredLen The number of bytes to copy from the copy buffer to the logger buffer.
void prvUpdateFromCopyBuffer( uint16_t usRequiredLen )
{
    uint16_t usDistanceToEndBuffer, usNextWriteIndex;

    usDistanceToEndBuffer = LOGGER_BUF_LEN - xLoggerData.usWriteIndex;

    if( usRequiredLen > usDistanceToEndBuffer )
    {
        memcpy(
            &xLoggerData.pcBuffer[ xLoggerData.usWriteIndex ],
            xLoggerData.pcCopyBuffer,
            usDistanceToEndBuffer );
        memcpy(
            xLoggerData.pcBuffer,
            &xLoggerData.pcCopyBuffer[ usDistanceToEndBuffer ],
            usRequiredLen - usDistanceToEndBuffer );
    }
    else
    {
        memcpy(
            &xLoggerData.pcBuffer[ xLoggerData.usWriteIndex ],
            xLoggerData.pcCopyBuffer,
            usRequiredLen );
    }

    usNextWriteIndex = ( xLoggerData.usWriteIndex + usRequiredLen ) % LOGGER_BUF_LEN;
    xLoggerData.usWriteIndex = usNextWriteIndex;
}

/// @brief Print a string to the UART peripheral.
/// @param pcStr 
/// @param usLen 
void prvPutString( char * const pcStr,
                   uint32_t usLen )
{
    HAL_UART_Transmit(
        &huart3,
        ( uint8_t * ) pcStr,
        usLen,
        ( TickType_t ) portMAX_DELAY );
}

#define TIMEOUT_STRING    "[Warning] Detected timeout in debug output\r\n"

/// @brief Task that periodically prints the logger buffer contents to a UART peripheral.
/// @param pvParameters 
void prvLoggerTask( void * const pvParameters )
{
    int16_t sBytesToRead;
    uint16_t usDistanceToEndBuffer;

    for( ; ; )
    {
        /* wait for buffer */

        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

        if( xLoggerData.ucTimedOut != 0 )
        {
            prvPutString( TIMEOUT_STRING, sizeof( TIMEOUT_STRING ) );
            xLoggerData.ucTimedOut = 0;
        }

        sBytesToRead = xLoggerData.usWriteIndex - xLoggerData.usReadIndex;
        usDistanceToEndBuffer = LOGGER_BUF_LEN - xLoggerData.usReadIndex;

        if( sBytesToRead > 0 )
        {
            prvPutString(
                &xLoggerData.pcBuffer[ xLoggerData.usReadIndex ],
                sBytesToRead );
        }
        else
        {
            prvPutString(
                &xLoggerData.pcBuffer[ xLoggerData.usReadIndex ],
                usDistanceToEndBuffer );
            prvPutString(
                xLoggerData.pcBuffer,
                xLoggerData.usWriteIndex );
        }

        xLoggerData.usReadIndex += sBytesToRead;
    }
}
