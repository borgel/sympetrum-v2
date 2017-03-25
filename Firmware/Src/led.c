
/**
 */
#include "led.h"
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


//SPI_HandleTypeDef hspi1;

bool led_Init(SPI_HandleTypeDef spiBus) {
   state.spi = spiBus;

   memset(state.leds, 0, sizeof(state.leds) / sizeof(state.leds[0]));
   //now add back the headers
   for(int i = 0; i < MAX_LED_CHAIN_LENGTH; i++) {
      state.leds[i].globalHeader = 0x1F;
   }

   //note, SPI init is done in main
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

   //FIXME rm
   iprintf("Updating LEDs...\n");

   //TODO we have to strip the const here. That's ok, right? Read the SRC
   //FIXME what is a good timeout here?
   HAL_SPI_Transmit(&state.spi, (uint8_t*)LED_FRAME_START, sizeof(LED_FRAME_START), 10000);

   for(i = 0; i < MAX_LED_CHAIN_LENGTH; i++) {
      HAL_SPI_Transmit(&state.spi, state.leds[i].raw, 4, 10000);
   }

   HAL_SPI_Transmit(&state.spi, (uint8_t*)LED_FRAME_STOP, sizeof(LED_FRAME_STOP), 10000);

   return true;
}

