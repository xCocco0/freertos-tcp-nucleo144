
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "cmsis_os.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "peripherals.h"

#ifdef DEBUG
//#include "printf_stdarg.h"
#include "logger.h"
#else
#define vLoggerInit()
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
		configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3
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

#define mainIPv6_ADRR            "fe80::dead:beef"
#define mainIPv6_PREFIX          "fe80::"
#define mainIPv6_DNS_SERVER_ADDR "2001:4860:4860::8888"        /* Google DNS */
#define mainIPv6_GATEWAY_ADDR    "fe80::4535:c3a4:9457:6e5a"

NetworkInterface_t xInterfaces[1];
NetworkEndPoint_t xEndPoints[1];

/* Private function prototypes -----------------------------------------------*/

extern void vTaskUDPSendIPv4(void *argument);
extern void vTaskUDPSendIPv6(void *argument);
extern void vTaskTCPSendIPv4(void *argument);

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
		MX_CRC_Init();

		vLoggerInit();

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
		
		IPv6_Address_t xIPAddress, xPrefix, xGateWay, xDNSServer;

		FreeRTOS_inet_pton6( mainIPv6_PREFIX, xPrefix.ucBytes );
		FreeRTOS_inet_pton6( mainIPv6_DNS_SERVER_ADDR, xDNSServer.ucBytes );
		FreeRTOS_inet_pton6( mainIPv6_ADRR, xIPAddress.ucBytes );
		FreeRTOS_inet_pton6( mainIPv6_GATEWAY_ADDR, xGateWay.ucBytes );
/*
		FreeRTOS_FillEndPoint_IPv6( &( xInterfaces[ 0 ] ),
						&( xEndPoints[ 1 ] ),
						&( xIPAddress ),
						&( xPrefix ),
						64,            // Prefix length.
						&( xGateWay ),
						&( xDNSServer ), // pxDNSServerAddress: Not used yet.
						ucMACAddress );*/

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
		//
		BaseType_t xRet;
		//xRet = xTaskCreate(vTaskUDPSendIPv4, "Def", 1024, NULL, tskIDLE_PRIORITY+1, NULL);
		//xRet = xTaskCreate(vTaskUDPSendIPv6, "Def", 1024, NULL, tskIDLE_PRIORITY+1, NULL);
		xRet = xTaskCreate(vTaskTCPSendIPv4, "Def", 256, NULL, tskIDLE_PRIORITY+1, NULL);
		vTaskStartScheduler();

		/* Infinite loop */

		configPRINTF( ("Warning! Scheduler returned\r\n") );
		for(;;);
}

/*
// required by printf_stdarg
void vOutputChar( const char cChar, const TickType_t xTicksToWait )
{
		HAL_UART_Transmit(&huart3, (uint8_t *)&cChar, 1, (uint32_t) xTicksToWait);
}
*/
