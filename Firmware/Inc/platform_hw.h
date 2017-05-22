#ifndef PLATFORM_HW_H__
#define PLATFORM_HW_H__

#include <stdbool.h>
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"

#define  USER_BUTTON_PORT        (GPIOA)
#define  USER_BUTTON_PIN         (GPIO_PIN_0)

#define  LED_SPI_INSTANCE        (SPI1)

bool platformHW_Init(void);
bool platformHW_SpiInit(SPI_HandleTypeDef * const spi, SPI_TypeDef* spiInstance);


#endif//PLATFORM_HW_H__

