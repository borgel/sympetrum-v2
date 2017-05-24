
/**
 */
#include "led.h"
#include "platform_hw.h"
#include "color.h"
#include "iprintf.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_spi.h"

#include "baf/baf.h"
#include "yabi/yabi.h"

#include <string.h>

#define YABI_CHANNELS      (LED_CHAIN_LENGTH * 3)

static uint8_t const DefaultTransitionTimeMS = 100;

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
   SPI_HandleTypeDef          spi;

   //this is crappy. We store BOTH the HSV state (for math) and the RGB state
   //(for sending).
   union led_Data             leds[LED_CHAIN_LENGTH];
   struct color_ColorHSV      ledsHSV[LED_CHAIN_LENGTH];

   //the data which backs YABI's channels. Arranged in groups of 3. The grouping
   //goes like {H, S, V}{H, S, V}, etc. So for 10 LEDs we have 30 'channels'. Modulo
   //math is used to figure out which is which at channel-set time.
   struct yabi_ChannelRecord  yabiBacking[YABI_CHANNELS];
};
static struct led_State state;

static void* const led_HwInit(void);
static void led_SetChannelMulti(yabi_ChanID chan, yabi_ChanValue value);
static void led_UpdateChannels(yabi_FrameID frame);

bool led_Init(void) {
   yabi_Error yres;

   struct yabi_Config yc = {
      .frameStartCB           = NULL,
      .frameEndCB             = led_UpdateChannels,
      .channelChangeCB        = led_SetChannelMulti,
      .channelChangeGroupCB   = NULL,        //TODO can we provide this?
      .hwConfig = {
         .setup               = led_HwInit,
         .teardown            = NULL,
         .hwConfig            = NULL,
      },
   };

   struct yabi_ChannelStateConfiguration const csc = {
      .channelStorage = state.yabiBacking,
      .numChannels = YABI_CHANNELS,
   };

   // setup the channel interpolator
   yres = yabi_init(&yc, &csc);
   if(yres != YABI_OK) {
      iprintf("YABI init returned %d\n", yres);
      return false;
   }

   //start the interpolator (we'll leave it running forever). This triggers an init of the LED HW
   yabi_setStarted(true);

   return true;
}

/*
 * Externally available hook to set color stuff.
 */
bool led_SetChannel(uint32_t id, struct color_ColorHSV c) {
   yabi_Error res;

   //we need to explode this HSV object into the three components YABI needs
   res  = yabi_setChannel((id * 3) + 0, c.h, DefaultTransitionTimeMS);
   res |= yabi_setChannel((id * 3) + 1, c.s, DefaultTransitionTimeMS);
   res |= yabi_setChannel((id * 3) + 2, c.v, DefaultTransitionTimeMS);
   if(res != YABI_OK) {
      return false;
   }
   return true;
}

/*
 * Used for BAF to reach in directly and control YABI
 */
//FIXME encapsulate this better/rename it?
bool led_SetSubChannel(uint32_t id, yabi_ChanValue val, uint32_t timeMS) {
   yabi_Error res;

   res  = yabi_setChannel(id, val, timeMS);
   if(res != YABI_OK) {
      return false;
   }
   return true;
}

static void* const led_HwInit(void) {
   //start SPI
   if(platformHW_SpiInit(&state.spi, LED_SPI_INSTANCE)) {
      return NULL;
   }

   //wipe out our state struct
   memset(state.leds, 0, sizeof(state.leds) / sizeof(state.leds[0]));
   memset(state.ledsHSV, 0, sizeof(state.ledsHSV) / sizeof(state.ledsHSV[0]));
   //now add back the headers
   for(int i = 0; i < LED_CHAIN_LENGTH; i++) {
      state.leds[i].globalHeader = 0x1F;
   }

   return NULL;
}

/*
 * This is the hook Yabi calls to set a channel. Yabi doesn't know about HSV, so
 * it uses a mapping scheme to control each parameter. This function is called by
 * Yabi, applies the mapping, then applies the change to the LEDs.
 * The mapping is the obvious one:
 * 0 - H
 * 1 - S
 * 2 - V
 *
 * Example channel math for Yabi setting channel 28
 * 28/3 = 9 (9th LED)
 * 28%3 = 1 (S conponent)
 */
//FIXME rename
static void led_SetChannelMulti(yabi_ChanID chan, yabi_ChanValue value) {
   uint8_t const realChan = (chan / 3);
   struct color_ColorHSV * const hsv = &state.ledsHSV[realChan];
   switch(chan % 3) {
      case 0:
         hsv->h = value;
      case 1:
         hsv->s = value;
      case 2:
         hsv->v = value;
      default:
         break;
   }

   //now apply the HSV array to the RGB array
   color_HSV2RGB(hsv, &state.leds[realChan].color);
}

static void led_UpdateChannels(yabi_FrameID frame) {
   int i;

   (void)frame;

   //TODO we have to strip the const here. That's ok, right? Read the SRC
   //TODO what is a better timeout here?
   HAL_SPI_Transmit(&state.spi, (uint8_t*)LED_FRAME_START, sizeof(LED_FRAME_START), 10000);

   for(i = 0; i < LED_CHAIN_LENGTH; i++) {
      HAL_SPI_Transmit(&state.spi, state.leds[i].raw, 4, 10000);
   }

   HAL_SPI_Transmit(&state.spi, (uint8_t*)LED_FRAME_STOP, sizeof(LED_FRAME_STOP), 10000);
}

void led_GiveTime(uint32_t systimeMS) {
   //TODO animation from (fake, anim) clock. Set bias in animation

   //FYI: the NULL is time until next call. Not useful without threads
   baf_giveTime(systimeMS, NULL);
   yabi_giveTime(systimeMS);
}

