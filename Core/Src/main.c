/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "peripherals.h"

#include "FreeRTOS_TSN_NetworkScheduler.h"
#include "FreeRTOS_TSN_Controller.h"
#include "FreeRTOS_TSN_VLANTags.h"
#include "FreeRTOS_TSN_Sockets.h"
#include "FreeRTOS_TSN_Timebase.h"

#include "NetworkWrapper.h"

#ifdef DEBUG
/*#include "printf_stdarg.h" */
    #include "logger.h"
#else
    #define vLoggerInit()
#endif

#define mainIS_MASTER    0

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

const uint8_t ucIPAddress[ ipIP_ADDRESS_LENGTH_BYTES ] =
{
    configIP_ADDR0, configIP_ADDR1, configIP_ADDR2,
    #if ( mainIS_MASTER == 1 )
        configIP_ADDR3
    #else
        configIP_ADDR3 + 1
    #endif
};
const uint8_t ucNetMask[ ipIP_ADDRESS_LENGTH_BYTES ] =
{
    configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3
};
const uint8_t ucGatewayAddress[ ipIP_ADDRESS_LENGTH_BYTES ] =
{
    configGATEWAY_ADDR0, configGATEWAY_ADDR1, configGATEWAY_ADDR2, configGATEWAY_ADDR3
};
const uint8_t ucDNSServerAddress[ ipIP_ADDRESS_LENGTH_BYTES ] =
{
    configDNS_SERVER_ADDR0, configDNS_SERVER_ADDR1, configDNS_SERVER_ADDR2, configDNS_SERVER_ADDR3
};
const uint8_t ucMACAddress[ ipMAC_ADDRESS_LENGTH_BYTES ] =
{
    configMAC_ADDR0, configMAC_ADDR1, configMAC_ADDR2,
    configMAC_ADDR3, configMAC_ADDR4,
    #if ( mainIS_MASTER == 1 )
        configMAC_ADDR5
    #else
        configMAC_ADDR5 + 1
    #endif
};

#define mainIPv6_ADRR               "fe80::dead:beef"
#define mainIPv6_PREFIX             "fe80::"
#define mainIPv6_DNS_SERVER_ADDR    "2001:4860:4860::8888"     /* Google DNS */
#define mainIPv6_GATEWAY_ADDR       "fe80::4535:c3a4:9457:6e5a"

NetworkInterface_t xInterfaces[ 1 ];
NetworkInterfaceConfig_t xInterfaceConfigs[ 1 ];
NetworkEndPoint_t xEndPoints[ 2 ];

TaskHandle_t xTaskLEDHandle;

/* Private function prototypes -----------------------------------------------*/

extern void vTaskUDPSendIPv4( void * argument );
extern void vTaskUDPSendIPv6( void * argument );
extern void vTaskTCPSendIPv4( void * argument );
extern void vTaskTSNTest( void * argument );
extern void vTaskTSNTestCBS( void * argument );
extern void vTaskSyncMaster( void * argument );
extern void vTaskSyncSlave( void * argument );

/**
 * @brief  The application entry point.
 * @retval int
 */
int main( void )
{
    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();
    PeriphCommonClock_Config();
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    /*MX_USART2_UART_Init(); */
    MX_USART3_UART_Init();
    MX_USB_OTG_FS_PCD_Init();
    MX_RNG_Init();
    MX_CRC_Init();
    MX_TIM2_Init();
    MX_TIM5_Init();

    vLoggerInit();

    configPRINTF( ( "\n\n\n\r#---------- Starting execution ----------#\r\n" ) );
    #if defined( __DATE__ ) && defined( __TIME__ ) && defined( DEBUG )
        configPRINTF( ( "# %-38s #\r\n", "Compiled: "__DATE__ " "__TIME__ ) );
        configPRINTF( ( "#----------------------------------------#\r\n" ) );
    #endif

    /* Networking configuration-------------------------------------------------*/

    configPRINTF( ( "Setting up network interface...\r\n" ) );
    pxTSN_FillInterfaceDescriptor( 0, &( xInterfaces[ 0 ] ), &( xInterfaceConfigs[ 0 ] ) );

    xInterfaceConfigs[ 0 ].xNumTags = 0; 
	#if 1
		xInterfaceConfigs[ 0 ].xNumTags = 2; 
		xInterfaceConfigs[ 0 ].usVLANTag = 0;
		xInterfaceConfigs[ 0 ].usServiceVLANTag = 0; 
		vlantagSET_PCP_FROM_TCI( xInterfaceConfigs[ 0 ].usVLANTag, vlantagCLASS_7 );
		vlantagSET_PCP_FROM_TCI( xInterfaceConfigs[ 0 ].usServiceVLANTag, vlantagCLASS_0 );
	#endif

    FreeRTOS_FillEndPoint(
        &( xInterfaces[ 0 ] ), &( xEndPoints[ 0 ] ), ucIPAddress,
        ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress );

    IPv6_Address_t xIPAddress, xPrefix, xGateWay, xDNSServer;

    FreeRTOS_inet_pton6( mainIPv6_PREFIX, xPrefix.ucBytes );
    FreeRTOS_inet_pton6( mainIPv6_DNS_SERVER_ADDR, xDNSServer.ucBytes );
    FreeRTOS_inet_pton6( mainIPv6_ADRR, xIPAddress.ucBytes );
    FreeRTOS_inet_pton6( mainIPv6_GATEWAY_ADDR, xGateWay.ucBytes );

    FreeRTOS_FillEndPoint_IPv6( &( xInterfaces[ 0 ] ),
                                &( xEndPoints[ 1 ] ),
                                &( xIPAddress ),
                                &( xPrefix ),
                                64,              /* Prefix length. */
                                &( xGateWay ),
                                &( xDNSServer ), /* pxDNSServerAddress: Not used yet. */
                                ucMACAddress );

    configPRINTF( ( "Done!\r\n" ) );

    configPRINTF( ( "Initializing IP stack...\r\n" ) );
    FreeRTOS_IPInit_Multi();
    configPRINTF( ( "Done!\r\n" ) );

    /* In future these should be moved elsewhere
     */
    vNetworkQueueInit();
    prvTSNController_Initialise();
    vInitialiseTSNSockets();
    vTimebaseInit();

    /* Task definitions---------------------------------------------------------*/

    BaseType_t xRet;
    /*xRet = xTaskCreate(vTaskUDPSendIPv4, "UDPSendIPv4", 1024, NULL, tskIDLE_PRIORITY+1, NULL); */
    /*xRet = xTaskCreate(vTaskUDPSendIPv6, "UDPSendIPv6", 1024, NULL, tskIDLE_PRIORITY+1, NULL); */
    /*xRet = xTaskCreate(vTaskTCPSendIPv4, "TCPSendIPv4", 1024, NULL, tskIDLE_PRIORITY+1, NULL); */
    /*xRet = xTaskCreate( vTaskTSNTest, "TSNTest", 1024, NULL, tskIDLE_PRIORITY + 1, NULL ); */
    xRet = xTaskCreate( vTaskTSNTestCBS, "TSNTestCBS", 1024, NULL, tskIDLE_PRIORITY + 1, NULL );

	#if 0
		#if ( mainIS_MASTER == 1 )
			xRet = xTaskCreate( vTaskSyncMaster, "Master", 1024, NULL, tskIDLE_PRIORITY + 1, NULL );
		#else
			xRet = xTaskCreate( vTaskSyncSlave, "Slave", 1024, NULL, tskIDLE_PRIORITY + 1, NULL );
		#endif
	#endif

    vTaskStartScheduler();

    /* Infinite loop */

    configPRINTF( ( "Warning! Scheduler returned\r\n" ) );

    for( ; ; )
    {
    }
}

/*
 * // required by printf_stdarg
 * void vOutputChar( const char cChar, const TickType_t xTicksToWait )
 * {
 *      HAL_UART_Transmit(&huart3, (uint8_t *)&cChar, 1, (uint32_t) xTicksToWait);
 * }
 */

void vTIM2_Callback( void )
{
    if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    {
        /*vLoggerPrintFromISR( ( "[TIM2] Tac\r\n" ) ); */
        if( __HAL_TIM_GET_COUNTER( &htim5 ) % 3 )
        {
            HAL_GPIO_WritePin( LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET );
        }
        else
        {
            HAL_GPIO_WritePin( LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET );
        }
    }
}

void vTIM5_Callback( void )
{
    if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    {
        /*HAL_GPIO_WritePin( LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET ); */
        /*vTaskNotifyGiveFromISR( xTaskLEDHandle, NULL ); */
        vLoggerPrintFromISR( ( "[TIM5] Tic\r\n" ) );
    }
}
