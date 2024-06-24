#include "peripherals.h"

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

RNG_HandleTypeDef hrng;

CRC_HandleTypeDef hcrc;

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config( void )
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE1 );

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    if( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_5 ) != HAL_OK )
    {
        Error_Handler();
    }
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config( void )
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_TIM;
    PeriphClkInitStruct.TIMPresSelection = RCC_TIMPRES_ACTIVATED;

    if( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK )
    {
        Error_Handler();
    }
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
void MX_USART3_UART_Init( void )
{
    /* USER CODE BEGIN USART3_Init 0 */

    /* USER CODE END USART3_Init 0 */

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if( HAL_UART_Init( &huart3 ) != HAL_OK )
    {
        Error_Handler();
    }

    /* USER CODE BEGIN USART3_Init 2 */

    /* USER CODE END USART3_Init 2 */
}

void MX_USART2_UART_Init( void )
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if( HAL_UART_Init( &huart2 ) != HAL_OK )
    {
        Error_Handler();
    }
}

/**
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
void MX_USB_OTG_FS_PCD_Init( void )
{
    /* USER CODE BEGIN USB_OTG_FS_Init 0 */

    /* USER CODE END USB_OTG_FS_Init 0 */

    /* USER CODE BEGIN USB_OTG_FS_Init 1 */

    /* USER CODE END USB_OTG_FS_Init 1 */
    hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
    hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
    hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
    hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
    hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
    hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;

    if( HAL_PCD_Init( &hpcd_USB_OTG_FS ) != HAL_OK )
    {
        Error_Handler();
    }

    /* USER CODE BEGIN USB_OTG_FS_Init 2 */

    /* USER CODE END USB_OTG_FS_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void MX_GPIO_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( GPIOB, LD1_Pin | LD3_Pin | LD2_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin : USER_Btn_Pin */
    MX_USART3_UART_Init();
    GPIO_InitStruct.Pin = USER_Btn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( USER_Btn_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : PA0 */

    /*
     * GPIO_InitStruct.Pin = GPIO_PIN_0;
     * GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
     * GPIO_InitStruct.Pull = GPIO_NOPULL;
     * HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
     */
    /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
    GPIO_InitStruct.Pin = /*LD1_Pin|*/ LD3_Pin | LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

    /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
    GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin : USB_OverCurrent_Pin */
    GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( USB_OverCurrent_GPIO_Port, &GPIO_InitStruct );

    /*Configure PD5 as USART2 TX*/
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    /*Configure PD6 as USART2 RX*/
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}


/**
 * @brief RNG Initialization Function
 * @param None
 * @retval None
 */
void MX_RNG_Init( void )
{
    /* USER CODE BEGIN RNG_Init 0 */

    /* USER CODE END RNG_Init 0 */

    /* USER CODE BEGIN RNG_Init 1 */

    /* USER CODE END RNG_Init 1 */
    hrng.Instance = RNG;

    if( HAL_RNG_Init( &hrng ) != HAL_OK )
    {
        Error_Handler();
    }

    /* USER CODE BEGIN RNG_Init 2 */

    /* USER CODE END RNG_Init 2 */
}

void MX_CRC_Init( void )
{
    /* USER CODE BEGIN CRC_Init 0 */

    /* USER CODE END CRC_Init 0 */

    /* USER CODE BEGIN CRC_Init 1 */

    /* USER CODE END CRC_Init 1 */
    hcrc.Instance = CRC;

    if( HAL_CRC_Init( &hcrc ) != HAL_OK )
    {
        Error_Handler();
    }

    /* USER CODE BEGIN CRC_Init 2 */

    /* USER CODE END CRC_Init 2 */
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM2_Init( void )
{
    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig = { 0 };

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 168000000;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if( HAL_TIM_Base_Init( &htim2 ) != HAL_OK )
    {
        Error_Handler();
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    if( HAL_TIM_ConfigClockSource( &htim2, &sClockSourceConfig ) != HAL_OK )
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if( HAL_TIMEx_MasterConfigSynchronization( &htim2, &sMasterConfig ) != HAL_OK )
    {
        Error_Handler();
    }

    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/**
 * @brief TIM5 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM5_Init( void )
{
    /* USER CODE BEGIN TIM5_Init 0 */

    /* USER CODE END TIM5_Init 0 */

    TIM_SlaveConfigTypeDef sSlaveConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig = { 0 };

    /* USER CODE BEGIN TIM5_Init 1 */

    /* USER CODE END TIM5_Init 1 */
    htim5.Instance = TIM5;
    htim5.Init.Prescaler = 0;
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim5.Init.Period = 1000;
    htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if( HAL_TIM_Base_Init( &htim5 ) != HAL_OK )
    {
        Error_Handler();
    }

    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
    sSlaveConfig.InputTrigger = TIM_TS_ITR0;

    if( HAL_TIM_SlaveConfigSynchro( &htim5, &sSlaveConfig ) != HAL_OK )
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if( HAL_TIMEx_MasterConfigSynchronization( &htim5, &sMasterConfig ) != HAL_OK )
    {
        Error_Handler();
    }

    /* USER CODE BEGIN TIM5_Init 2 */

    /* USER CODE END TIM5_Init 2 */
}
