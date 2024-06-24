#include "peripherals.h"

#include "FreeRTOS.h"

#include "FreeRTOS_TSN_Timebase.h"

#define SMALL_FREQ        ( 168000000UL )
#define SMALL_FREQ_MHZ    ( 168UL )

#define BIG_FREQ          ( 1 )

#define NS_TO_TIM2( ns )      ( ( uint32_t ) ( ( ns ) * ( ( double ) SMALL_FREQ_MHZ / 1000 ) ) )
#define SEC_TO_TIM5( sec )    ( ( uint32_t ) ( ( sec ) * BIG_FREQ ) )

#define TIM2_TO_NS( cnt )     ( ( uint32_t ) ( ( cnt ) * ( ( double ) 1000 / SMALL_FREQ_MHZ ) ) )
#define TIM5_TO_SEC( cnt )    ( ( uint32_t ) ( ( cnt ) / BIG_FREQ ) )

static void prvStart( void )
{
    HAL_TIM_Base_Start_IT( &htim5 );
    HAL_TIM_Base_Start_IT( &htim2 );
}

static void prvStop( void )
{
    /* Stopping TIM2 is enough */
    HAL_TIM_Base_Stop_IT( &htim2 );
    HAL_TIM_Base_Start_IT( &htim5 );
}

static void prvSetTime( const struct freertos_timespec * ts )
{
    prvStop();

    __HAL_TIM_SET_COUNTER( &htim2, NS_TO_TIM2( ts->tv_nsec ) );
    __HAL_TIM_SET_COUNTER( &htim5, SEC_TO_TIM5( ts->tv_sec ) );

    prvStart();
}

static void prvGetTime( struct freertos_timespec * ts )
{
    uint32_t temp;

    do
    {
        temp = TIM5_TO_SEC( __HAL_TIM_GET_COUNTER( &htim5 ) );       /*2.9 */
        ts->tv_nsec = TIM2_TO_NS( __HAL_TIM_GET_COUNTER( &htim2 ) ); /*3.1 */
        ts->tv_sec = TIM5_TO_SEC( __HAL_TIM_GET_COUNTER( &htim5 ) ); /*3.2 */
    }
    while( temp != ts->tv_sec );
}

void vTimebaseInit( void )
{
    TimebaseHandle_t xTb;

    xTb.fnStart = prvStart;
    xTb.fnStop = prvStop;
    xTb.fnSetTime = prvSetTime;
    xTb.fnGetTime = prvGetTime;

    configASSERT( xTimebaseHandleSet( &xTb ) != pdFAIL );

    vTimebaseStart();
}
