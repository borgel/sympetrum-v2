#ifndef PLATFORM_HW_H__
#define PLATFORM_HW_H__

#include <stdbool.h>
#include "color.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"

#define LED_CHAIN_LENGTH       10

#define  USER_BUTTON_PORT        (GPIOA)
#define  USER_BUTTON_PIN         (GPIO_PIN_0)

#define  LED_SPI_INSTANCE        (SPI1)

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
union platformHW_LEDRegister {
   uint8_t  raw[4];
   struct {
      //3 bits always, 5 bits global brightness, 8B, 8G, 8R
      //Glob = 0xE1 = min bright
      uint8_t                 globalBrightness  :5;

      //header/global brightness is 0bAAABBBBB
      //A = 1
      //B = integer brightness divisor from 0x0 -> 0x1F
      uint8_t const                 header      :3;

      struct color_ColorRGB   color;
   };
};
#pragma pack(pop)   /* restore original alignment from stack */


bool platformHW_Init(void);
bool platformHW_SpiInit(SPI_HandleTypeDef * const spi, SPI_TypeDef* spiInstance);

void platformHW_UpdateLEDs(SPI_HandleTypeDef* spi);


#endif//PLATFORM_HW_H__

