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

#define TIM2_PERIOD_TICKS    ( 168000000UL )

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

static void prvAddOffset( struct freertos_timespec * ts,
                          BaseType_t xPositive )
{
    uint32_t register ulOffset2, ulOffset5, ulTime2, ulTime5;

    ulOffset2 = NS_TO_TIM2( ts->tv_nsec );
    ulOffset5 = SEC_TO_TIM5( ts->tv_sec );

    prvStop();

    ulTime2 = __HAL_TIM_GET_COUNTER( &htim2 );
    ulTime5 = __HAL_TIM_GET_COUNTER( &htim5 );

    if( xPositive != pdFALSE )
    {
        /* Note that this will never overflow the 32 bits registers */
        ulTime2 = ulTime2 + ulOffset2;

        if( ulTime2 > TIM2_PERIOD_TICKS )
        {
            ++ulOffset5;
            ulTime2 %= TIM2_PERIOD_TICKS;
        }

        ulTime5 = ulTime5 + ulOffset5;
    }
    else
    {
        if( ulTime2 >= ulOffset2 )
        {
            ulTime2 -= ulOffset2;
        }
        else
        {
            ulTime2 = TIM2_PERIOD_TICKS - ( ulOffset2 - ulTime2 );
            ++ulOffset5;
        }

        ulTime5 = ( ulTime5 >= ulOffset5 ) ? ( ulTime5 - ulOffset5 ) : ( 0 );
    }

    __HAL_TIM_SET_COUNTER( &htim2, ulTime2 );
    __HAL_TIM_SET_COUNTER( &htim5, ulTime5 );

    prvStart();
}

void vTimebaseInit( void )
{
    TimebaseHandle_t xTb;

    xTb.fnStart = prvStart;
    xTb.fnStop = prvStop;
    xTb.fnSetTime = prvSetTime;
    xTb.fnGetTime = prvGetTime;
    xTb.fnAdjTime = prvAddOffset;

    configASSERT( xTimebaseHandleSet( &xTb ) != pdFAIL );

    vTimebaseStart();
}
