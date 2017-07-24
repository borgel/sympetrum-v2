
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
#include <stdlib.h>

#define YABI_CHANNELS      (LED_CHAIN_LENGTH * 3)

static uint8_t const DefaultTransitionTimeMS = 100;
struct led_State {
   SPI_HandleTypeDef             spi;

   //this is crappy. We store BOTH the HSV state (for math) and the RGB state
   struct color_ColorHSV         ledsHSV[LED_CHAIN_LENGTH];

   //the data which backs YABI's channels. Arranged in groups of 3. The grouping
   //goes like {H, S, V}{H, S, V}, etc. So for 10 LEDs we have 30 'channels'. Modulo
   //math is used to figure out which is which at channel-set time.
   struct yabi_ChannelRecord     yabiBacking[YABI_CHANNELS];
};
static struct led_State state;

extern union platformHW_LEDRegister  LedRegisterStates[LED_CHAIN_LENGTH];

static baf_ChannelID animationChannelIDs[LED_CHAIN_LENGTH] = {0};
// This is the 'animation' that the system runs
static struct baf_Animation AnimRGBFade = {
   .id                     = 1,
   .numSteps               = 1,
   .timeStepMS             = 5000,     //how long to wait between setting color targets
   .type                   = BAF_ASCHED_SIMPLE_RANDOM_LOOP,

   .aRandomSimpleLoop      = {
      .id                  = animationChannelIDs,
      .idLen               = LED_CHAIN_LENGTH,
      .transitionTimeMS     = 5000,    //how quickly to move towards the target color
      .params              = {
         .maxValue         = 255,      //255 is the max hue
         .minValue         = 0,
         .biasValue        = 0,        //these will be programmatically manipulated to set color tendencies
         .biasWeight       = 0,
      },
   },
};

static void* const led_HwInit(void);
static void led_YabiSetChannelCB(yabi_ChanID chan, yabi_ChanValue value);
static void led_UpdateChannels(yabi_FrameID frame);
static uint32_t bafRNGCB(uint32_t range);
static void bafChanGroupSetCB(struct baf_ChannelSetting const * const channels, baf_ChannelValue* const values, uint32_t num);
static void bafAnimStartCB(struct baf_Animation const * anim);
static void bafAnimStopCB(struct baf_Animation const * anim);
static yabi_ChanValue rolloverInterpolator(yabi_ChanValue current, yabi_ChanValue start, yabi_ChanValue end, float fraction, float absoluteFraction);

/*
 * Wire up the animation framework. It's composed of two parts:
 * BAF - High level triggering to set LEDs to certain values at a timer interval.
 * YABI - Interpolates between those points to create dank RGB fading action.
 *
 * The animation objects must be statically allocated, so ours is statically allocated
 *    up at the top of the file.
 */
bool led_Init(void) {
   yabi_Error yres;
   baf_Error bres;

   struct baf_Config bc = {
      .rngCB               = bafRNGCB,
      .animationStartCB    = bafAnimStartCB,
      .animationStopCB     = bafAnimStopCB,
      .setChannelGroupCB   = bafChanGroupSetCB,
   };

   struct yabi_Config yc = {
      .frameStartCB           = NULL,
      .frameEndCB             = led_UpdateChannels,
      .channelChangeCB        = led_YabiSetChannelCB,
      .channelChangeGroupCB   = NULL,        //TODO can we provide this?
      .interpolator           = rolloverInterpolator,
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

   // start the interpolator (we'll leave it running forever). This triggers an init of the LED HW
   yabi_setStarted(true);

   //FIXME rm
   struct color_ColorHSV c = {.h = 0, .s = 254, .v = 10};

   // prepare to start BAF later
   for(int i = 0; i < YABI_CHANNELS; i++) {
      //wire up BAF so it's channels are YABI's Hue's
      if(i % 3 == 0) {
         iprintf("Setting BAF channel ID %d to YABI %d\r\n", i/3, i);

         animationChannelIDs[i/3] = i;

         //FIXME rm?
         //set everyone's saturation to 100 and brightness to 10%
         led_SetChannelTimed(i/3, c, 10);
      }
   }

   // setup the animation framework
   bres = baf_init(&bc);
   if(bres != BAF_OK) {
      iprintf("BAF init returned %d\n", bres);
      return false;
   }
   return true;
}

/*
 * This system is NOT general purpose. This starts the RGB fade animation w/
 * Sympetrum algorithm.
 */
bool led_StartAnimation(void) {
   //start the random animation
   return BAF_OK == baf_startAnimation(&AnimRGBFade, BAF_ASTART_IMMEDIATE);
}

void led_SetBiasWeight(uint8_t biasWeight) {
   if(biasWeight > 100) {
      iprintf("Nonsense bias weight %d is larger than 100\n", biasWeight);
      return;
   }

   AnimRGBFade.aRandomSimpleLoop.params.biasWeight = biasWeight;
}
void led_SetBiasValue(uint8_t biasValue) {
   AnimRGBFade.aRandomSimpleLoop.params.biasValue = biasValue;
}

/*
 * Conditionally set the speed of either the per-channel progression and/or the overal
 * animation 'frame' rate.
 */
void led_SetAnimationSpeeds(uint32_t frameTime, uint32_t transitionTime) {
   if(frameTime) {
      AnimRGBFade.timeStepMS = frameTime;
   }
   if(transitionTime) {
      AnimRGBFade.aRandomSimpleLoop.transitionTimeMS = transitionTime;
   }
}

static yabi_ChanValue rolloverInterpolator(yabi_ChanValue current, yabi_ChanValue start, yabi_ChanValue end, float fraction, float absoluteFraction) {
   bool increasing;
   uint32_t error = 0;
   uint8_t mod = 0;

   if(end > start)   // XXX increasing
   {
      increasing = true;

      if( end - start > (start + 0xFF) - end) {
         mod = 0xFF;
         increasing = false;
      }
   }
   else     // XXX decreasing
   {
      increasing = false;

      if( start - end > (end + 0xFF) - start) {
         mod = 0xFF;
         increasing = true;
      }
   }

   if(increasing) {
      // what's the absolute value we should be at now?
      error = (uint32_t)(absoluteFraction * (float)((float)(end + mod) - (float)start));
      // what's the difference between that and the current value?
      error = error - current;

      return (uint8_t)(current + error);
   }
   else {
      error = (uint32_t)((1.0 - absoluteFraction) * (float)((float)(start + mod) - (float)end));
      error = current - error;

      return (uint8_t)(current - error);
   }
}

bool led_SetChannelTimed(uint32_t id, struct color_ColorHSV c, uint32_t timeMS) {
   yabi_Error res;

   iprintf("transition time %dms\n", timeMS);

   //we need to explode this HSV object into the three components YABI needs
   res  = yabi_setChannel((id * 3) + 0, c.h, timeMS);
   res |= yabi_setChannel((id * 3) + 1, c.s, timeMS);
   res |= yabi_setChannel((id * 3) + 2, c.v, timeMS);
   if(res != YABI_OK) {
      return false;
   }
   return true;
}

/*
 * Externally available hook to set color stuff.
 */
bool led_SetChannel(uint32_t id, struct color_ColorHSV c) {
   return led_SetChannelTimed(id, c, DefaultTransitionTimeMS);
}

static uint32_t bafRNGCB(uint32_t range) {
   return rand() % range;
}

// shim to connect BAF's channel group setting API to YABI's one-at-a-time API
static void bafChanGroupSetCB(struct baf_ChannelSetting const * const channels, baf_ChannelValue* const values, uint32_t num) {
   for(int i = 0; i < num; i++) {
      //FIXME rm
      //iprintf("\tSet Chan #%d to %d in %dms\n", channels[i].id, values[i], channels[i].transitionTimeMS);

      if(YABI_OK != yabi_setChannel(channels[i].id, values[i], channels[i].transitionTimeMS)) {
         //TODO handle?
         iprintf("Failed to set yabi channel value!\n");
      }
   }
}

static void bafAnimStartCB(struct baf_Animation const * anim) {
   iprintf("Animation #%u Start\n", anim->id);
   //TODO wire?
}
static void bafAnimStopCB(struct baf_Animation const * anim) {
   iprintf("Animation #%u Stop\n", anim->id);
   //TODO wire?
}

static void* const led_HwInit(void) {
   //start SPI
   if(!platformHW_SpiInit(&state.spi, LED_SPI_INSTANCE)) {
      return NULL;
   }

   //wipe out our state struct
   memset(state.ledsHSV, 0, sizeof(state.ledsHSV) / sizeof(state.ledsHSV[0]));
   //TODO anything else to clear?

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
static void led_YabiSetChannelCB(yabi_ChanID chan, yabi_ChanValue value) {
   uint8_t const realChan = (chan / 3);
   struct color_ColorHSV * const hsv = &state.ledsHSV[realChan];

   switch(chan % 3) {
      case 0:
         hsv->h = value;
         break;
      case 1:
         hsv->s = value;
         break;
      case 2:
         hsv->v = value;
         break;
      default:
         break;
   }

   //now apply the HSV array directly to the RGB array (in platform_hw)
   color_HSV2RGB(hsv, &LedRegisterStates[realChan].color);
}

/*
 * Shim to connect ot platform_hw backend.
 */
static void led_UpdateChannels(yabi_FrameID frame) {
   (void)frame;
   platformHW_UpdateLEDs(&state.spi);
}

void led_GiveTime(uint32_t systimeMS) {
   //FYI: the NULL is time until next call. Not useful without threads
   baf_giveTime(systimeMS, NULL);
   yabi_giveTime(systimeMS);
}

