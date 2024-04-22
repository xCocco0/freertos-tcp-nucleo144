
#include "logger.h"
#include "peripherals.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define LOGGER_BUF_LEN 128U
#define LOGGER_TASK_PRIORITY tskIDLE_PRIORITY+1
#define LOGGER_STACK_SIZE LOGGER_BUF_LEN + 64

#define PRINTER_TICKS_TO_WAIT (TickType_t) 1U

static struct {

		SemaphoreHandle_t xLock;
		char pcBuffer[LOGGER_BUF_LEN];
		char pcCopyBuffer[LOGGER_BUF_LEN];
		uint16_t usWriteIndex;

		uint16_t usReadIndex;

} xLoggerData;

TaskHandle_t xLoggerTaskHandle = NULL;
StaticTask_t xLoggerTCB;
StackType_t xLoggerStack[LOGGER_STACK_SIZE];

StaticSemaphore_t xLoggerSemaphoreBuffer;

void prvUpdateFromCopyBuffer(uint16_t usRequiredLen);
void prvPutString(char * const pcStr, uint32_t usLen);

/*
 * This function will periodically check
 * the Logger buffer and print the values
 */
void prvLoggerTask(void * const pvParameters);

/**** public functions ****/

void vLoggerInit(void) {
		xLoggerTaskHandle = xTaskCreateStatic(prvLoggerTask,
						"Logger task",
						LOGGER_STACK_SIZE,
						NULL,
						LOGGER_TASK_PRIORITY,
						xLoggerStack,
						&xLoggerTCB
						);
		xLoggerData.xLock = xSemaphoreCreateMutexStatic(&xLoggerSemaphoreBuffer);
		xLoggerData.usWriteIndex = 0;
		xLoggerData.usReadIndex = 0;

}

void vLoggerPrint(const char *format, ...)
{	
		va_list args;
		uint16_t usRequiredLen, sMaxLen;

		va_start( args, format );

		if( xSemaphoreTake( xLoggerData.xLock, PRINTER_TICKS_TO_WAIT ) != pdFALSE) {

				sMaxLen = xLoggerData.usReadIndex - xLoggerData.usWriteIndex;
				if( sMaxLen <= 0 ) sMaxLen += LOGGER_BUF_LEN;

				usRequiredLen = vsnprintf(
								xLoggerData.pcCopyBuffer,
								LOGGER_BUF_LEN,
								format,
								args );
				usRequiredLen = configMIN( usRequiredLen, sMaxLen );

				( void ) prvUpdateFromCopyBuffer(usRequiredLen);
			
				xTaskNotifyGive( xLoggerTaskHandle );
				xSemaphoreGive( xLoggerData.xLock );
		}

		va_end( args );
}

void vLoggerPrintline(const char *format, ...)
{	
		va_list args;
		uint16_t usRequiredLen, sMaxLen;

		va_start( args, format );

		if( xSemaphoreTake( xLoggerData.xLock, PRINTER_TICKS_TO_WAIT ) != pdFALSE) {

				sMaxLen = xLoggerData.usReadIndex - xLoggerData.usWriteIndex;
				if( sMaxLen <= 0 ) sMaxLen += LOGGER_BUF_LEN;

				usRequiredLen = vsnprintf(
								xLoggerData.pcCopyBuffer,
								LOGGER_BUF_LEN,
								format,
								args );
				usRequiredLen = configMIN( usRequiredLen, sMaxLen-2);

				char cChar;
			
				while( usRequiredLen > 0 ) {
						cChar = xLoggerData.pcCopyBuffer[usRequiredLen-1];
						if( cChar == '\r' || cChar == '\n' )
								usRequiredLen -= 1;
						else
								break;
				}

				xLoggerData.pcCopyBuffer[usRequiredLen] = '\r';
				xLoggerData.pcCopyBuffer[usRequiredLen+1] = '\n';
				usRequiredLen += 2;

				(void) prvUpdateFromCopyBuffer(usRequiredLen);
			
				xTaskNotifyGive( xLoggerTaskHandle );
				xSemaphoreGive( xLoggerData.xLock );
		}

		va_end( args );
}

/**** private functions ****/

void prvUpdateFromCopyBuffer(uint16_t usRequiredLen)
{

		uint16_t usDistanceToEndBuffer, usNextWriteIndex;

		usDistanceToEndBuffer = LOGGER_BUF_LEN - xLoggerData.usWriteIndex;

		if( usRequiredLen > usDistanceToEndBuffer) {
				memcpy(
						&xLoggerData.pcBuffer[xLoggerData.usWriteIndex],
						xLoggerData.pcCopyBuffer,
						usDistanceToEndBuffer );
				memcpy(
						xLoggerData.pcBuffer,
						&xLoggerData.pcCopyBuffer[usDistanceToEndBuffer],
						usRequiredLen - usDistanceToEndBuffer );
		} else {
				memcpy(
						&xLoggerData.pcBuffer[xLoggerData.usWriteIndex],
						xLoggerData.pcCopyBuffer,
						usRequiredLen );
		}
		usNextWriteIndex = (xLoggerData.usWriteIndex + usRequiredLen) % LOGGER_BUF_LEN;
		xLoggerData.usWriteIndex = usNextWriteIndex;
}

void prvPutString(char * const pcStr, uint32_t usLen)
{
		HAL_UART_Transmit(
				&huart3,
				(uint8_t *) pcStr,
				usLen,
				(TickType_t) portMAX_DELAY);

}

void prvLoggerTask(void * const pvParameters)
{
		int16_t sBytesToRead;
		uint16_t usDistanceToEndBuffer;

		for(;;) {
				configPRINTF( ("waiting for event\r\n") );
				// wait for buffer

				ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
				
				sBytesToRead = xLoggerData.usWriteIndex - xLoggerData.usReadIndex;
				usDistanceToEndBuffer = LOGGER_BUF_LEN - xLoggerData.usReadIndex;

				if( sBytesToRead > 0 ) {
						prvPutString(
								&xLoggerData.pcBuffer[xLoggerData.usReadIndex],
								sBytesToRead );
				} else {
						prvPutString(
								&xLoggerData.pcBuffer[xLoggerData.usReadIndex],
								usDistanceToEndBuffer );
						prvPutString(
								xLoggerData.pcBuffer,
								xLoggerData.usWriteIndex );
				}
				xLoggerData.usReadIndex += sBytesToRead;

		}
}
