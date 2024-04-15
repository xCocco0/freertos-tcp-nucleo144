
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "cmsis_os.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "peripherals.h"

#include "FreeRTOS_ARP.h" //TODO remove

#ifdef DEBUG
#include "printf_stdarg.h"
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

extern RNG_HandleTypeDef hrng;

osThreadId defaultTaskHandle;

const uint8_t ucIPAddress[ ipIP_ADDRESS_LENGTH_BYTES ] = {
		configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3
}; 
const uint8_t ucNetMask[ ipIP_ADDRESS_LENGTH_BYTES ] = {
		configNET_MASK0, configNET_MASK1, configNET_MASK2,configNET_MASK3
};
const uint8_t ucGatewayAddress[ ipIP_ADDRESS_LENGTH_BYTES ] = {
		configGATEWAY_ADDR0, configGATEWAY_ADDR1, configGATEWAY_ADDR2, configGATEWAY_ADDR3
};
const uint8_t ucDNSServerAddress[ ipIP_ADDRESS_LENGTH_BYTES ] = {
		configDNS_SERVER_ADDR0, configDNS_SERVER_ADDR1, configDNS_SERVER_ADDR2, configDNS_SERVER_ADDR3
};
const uint8_t ucMACAddress[ ipMAC_ADDRESS_LENGTH_BYTES ] = {
		configMAC_ADDR0, configMAC_ADDR1, configMAC_ADDR2,
		configMAC_ADDR3, configMAC_ADDR4, configMAC_ADDR5
};

NetworkInterface_t xInterfaces[1];
NetworkEndPoint_t xEndPoints[1];

/* Private function prototypes -----------------------------------------------*/
void StartDefaultTask(void *argument);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

		/* MCU Configuration--------------------------------------------------------*/

		/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
		HAL_Init();

		/* Configure the system clock */
		SystemClock_Config();

		/* Initialize all configured peripherals */
		MX_GPIO_Init();
		//MX_USART2_UART_Init();
		MX_USART3_UART_Init();
		MX_USB_OTG_FS_PCD_Init();
		MX_RNG_Init();

		configPRINTF( ("\n\n\n\r#---------- Starting execution ----------#\r\n") );
		#if defined(__DATE__) && defined(__TIME__) && defined(DEBUG)
		configPRINTF( ("# %-38s #\r\n", "Compiled: "__DATE__" "__TIME__) );
		configPRINTF( ("#----------------------------------------#\r\n") );
		#endif

		/* Networking configuration-------------------------------------------------*/

		configPRINTF( ("Setting up network interface...\r\n") );	
		pxFillInterfaceDescriptor(0, &(xInterfaces[0]));
		FreeRTOS_FillEndPoint(
						&(xInterfaces[0]), &(xEndPoints[0]), ucIPAddress,
						ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);

		configPRINTF( ("Done!\r\n") );

		configPRINTF( ("Initializing IP stack...\r\n") );
		FreeRTOS_IPInit_Multi();
		configPRINTF( ("Done!\r\n") );
		
		/* Task definitions---------------------------------------------------------*/

		/* Create the thread(s) */
		/* definition and creation of defaultTask */
		//osThreadDef(defaultTask, StartDefaultTask, osPriorityHigh, 0, 128);
		//defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

		/* Start scheduler */
		//osKernelStart();
		
		BaseType_t xRet = xTaskCreate(StartDefaultTask, "Def", 4*1024, NULL, tskIDLE_PRIORITY+1, NULL);
		vTaskStartScheduler();
		/* Infinite loop */

		configPRINTF( ("Warning! Scheduler returned\r\n") );
		for(;;);
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
/* USER CODE BEGIN 5 */
/* Infinite loop */
		configPRINTF( ("Running StartDefaultTask function\r\n") );
		for(;;) {
				
				configPRINTF( ("Creating socket...\r\n") );
				Socket_t xSock = FreeRTOS_socket(FREERTOS_AF_INET4,
								FREERTOS_SOCK_DGRAM, 
								FREERTOS_IPPROTO_UDP);
				if(xSock == FREERTOS_INVALID_SOCKET) {
						HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
						configPRINTF( ("Socket error\r\n") );
						for(;;);
				}
				configPRINTF( ("Done!\r\n") );
				
				struct freertos_sockaddr xSourceAddress;
				xSourceAddress.sin_addr = FreeRTOS_inet_addr_quick( ucIPAddress[ 0 ], ucIPAddress[ 1 ], ucIPAddress[ 2 ], ucIPAddress[ 3 ] );
				xSourceAddress.sin_port = FreeRTOS_htons(10000);
				xSourceAddress.sin_family = FREERTOS_AF_INET4;

				socklen_t xSize = sizeof(struct freertos_sockaddr);
				configPRINTF( ("Binding socket...\r\n") );
				BaseType_t xRet = FreeRTOS_bind(xSock, &xSourceAddress, xSize);
				if(xRet) {
						configPRINTF( ("Socket could not be bound: error %d\r\n", xRet) );
						for(;;);
				}
				configPRINTF( ("Done!\r\n") );

				struct freertos_sockaddr xDestinationAddress;
				//xDestinationAddress.sin_addr = FreeRTOS_inet_addr("169.254.151.41");
				xDestinationAddress.sin_addr = FreeRTOS_inet_addr("192.168.56.1");
				//xDestinationAddress.sin_addr = FreeRTOS_inet_addr("255.255.255.255");
				xDestinationAddress.sin_port = FreeRTOS_htons(10000);
				xDestinationAddress.sin_family = FREERTOS_AF_INET4;

				char cMsg[32];

				for(int i = 0; ; ++i) {
						sprintf(cMsg, "Hello world n.%d", i);
						configPRINTF( ("Sending message \"%s\"...", cMsg) );
						if(xIsIPInARPCache(xDestinationAddress.sin_addr))
							configPRINTF( ("[in cache]") );
						else
							configPRINTF( ("[not in cache]") );
						//FreeRTOS_OutputARPRequest_Multi( &(xEndPoints[0]),
						//				xDestinationAddress.sin_addr );
						BaseType_t xRet = FreeRTOS_sendto(xSock, cMsg, configMIN(sizeof(cMsg),strlen(cMsg)), 0,
								&xDestinationAddress, sizeof(xDestinationAddress)
						);
						configPRINTF( ("sent %d bytes.\r\n", xRet) );
						configASSERT(xRet >= 0);
						HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
						osDelay(1000UL);
						//vTaskDelay(1000UL / portTICK_PERIOD_MS);
				}
		}
}

void vOutputChar( const char cChar, const TickType_t xTicksToWait )
{
		HAL_UART_Transmit(&huart3, (uint8_t *)&cChar, 1, (uint32_t) xTicksToWait);
}

