
#ifndef __PERIPHERALS_H
#define __PERIPHERALS_H

#include "main.h"

// handles

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

extern RNG_HandleTypeDef hrng;

extern CRC_HandleTypeDef hcrc;

// prototypes

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USB_OTG_FS_PCD_Init(void);
void MX_RNG_Init(void);
void MX_CRC_Init(void);

#endif /* __PERIPHERALS_H */
