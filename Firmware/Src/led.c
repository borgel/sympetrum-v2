
/**
 */
#include "led.h"
#include "platform_hw.h"
#include "color.h"
#include "iprintf.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_spi.h"

#include <string.h>

static uint8_t const LED_FRAME_START[4] = {0x00, 0x00, 0x00, 0x00};
static uint8_t const LED_FRAME_STOP[4]  = {0xFF, 0xFF, 0xFF, 0xFF};

union led_Data {
   uint8_t  raw[4];
   struct {
      //header/global brightness is 0bAAABBBBB
      //A = 1
      //B = integer brightness divisor from 0x0 -> 0x1F

      //3 bits always, 5 bits global brightness, 8B, 8G, 8R
      //Glob = 0xE1 = min bright
      uint8_t                 globalHeader;
      struct color_ColorRGB   color;
   };
};

struct led_State {
   SPI_HandleTypeDef    spi;

   union led_Data       leds[MAX_LED_CHAIN_LENGTH];
};
static struct led_State state;


static bool spi_Init(SPI_HandleTypeDef * const spi, SPI_TypeDef* spiInstance);

bool led_Init(void) {
   //start SPI
   if(!spi_Init(&state.spi, LED_SPI_INSTANCE)) {
      return false;
   }

   memset(state.leds, 0, sizeof(state.leds) / sizeof(state.leds[0]));
   //now add back the headers
   for(int i = 0; i < MAX_LED_CHAIN_LENGTH; i++) {
      state.leds[i].globalHeader = 0x1F;
   }

   led_UpdateChannels();

   return true;
}

bool led_SetChannel(uint8_t ch, struct color_ColorRGB color) {
   if(ch > MAX_LED_CHAIN_LENGTH) {
      return false;
   }

   state.leds[ch].color = color;
   return true;
}

bool led_UpdateChannels(void) {
   int i;

   //TODO we have to strip the const here. That's ok, right? Read the SRC
   //FIXME what is a good timeout here?
   HAL_SPI_Transmit(&state.spi, (uint8_t*)LED_FRAME_START, sizeof(LED_FRAME_START), 10000);

   for(i = 0; i < MAX_LED_CHAIN_LENGTH; i++) {
      HAL_SPI_Transmit(&state.spi, state.leds[i].raw, 4, 10000);
   }

   HAL_SPI_Transmit(&state.spi, (uint8_t*)LED_FRAME_STOP, sizeof(LED_FRAME_STOP), 10000);

   return true;
}

/* SPI1 init function */
static bool spi_Init(SPI_HandleTypeDef * const spi, SPI_TypeDef* spiInstance)
{
   spi->Instance = spiInstance;
   spi->Init.Mode = SPI_MODE_MASTER;
   spi->Init.Direction = SPI_DIRECTION_2LINES;
   spi->Init.DataSize = SPI_DATASIZE_8BIT;
   spi->Init.CLKPolarity = SPI_POLARITY_LOW;
   spi->Init.CLKPhase = SPI_PHASE_1EDGE;
   spi->Init.NSS = SPI_NSS_SOFT;
   //spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
   spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; //for crappy saelae
   spi->Init.FirstBit = SPI_FIRSTBIT_MSB;
   spi->Init.TIMode = SPI_TIMODE_DISABLE;
   spi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
   spi->Init.CRCPolynomial = 7;
   spi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
   spi->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
   if (HAL_SPI_Init(spi) != HAL_OK)
   {
      return false;
   }
   return true;
}
