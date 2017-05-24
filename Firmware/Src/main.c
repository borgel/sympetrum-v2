/**
 */
#include "platform_hw.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_tim.h"
#include "iprintf.h"

#include "led.h"
#include "board_id.h"
#include "version.h"

#include "baf/baf.h"
#include "yabi/yabi.h"

#include "rc5_encode.h"
#include "rc5_decode.h"

#include <string.h>
#include <stdlib.h>

// statically allocated animation
static baf_ChannelID animationChannelIDs[30] = {0};
static struct baf_Animation AnimRGBFade = {
   .id                     = 1,
   .numSteps               = 1,
   .timeStepMS             = 1000,     //how long to wait between setting color targets
   .type                   = BAF_ASCHED_SIMPLE_RANDOM_LOOP,

   .aRandomSimpleLoop      = {
      .id                  = animationChannelIDs,
      .idLen               = 30,
      .transitionTimeMS     = 1000,    //how quickly to move towards the target color
      .params              = {
         .maxValue         = 255,      //255 is the max hue
         .minValue         = 0,
         .biasValue        = 0,        //these will be programmatically manipulated to set color tendencies
         .biasWeight       = 0,
      },
   },
};

static void VersionToLEDs(void);

static void initAnimationStack(void);
static uint32_t bafRNGCB(uint32_t range);
static void bafChanGroupSetCB(struct baf_ChannelSetting const * const channels, baf_ChannelValue* const values, uint32_t num);
static void bafAnimStartCB(struct baf_Animation const * anim);
static void bafAnimStopCB(struct baf_Animation const * anim);

int main(void)
{
   int i;

   // Reset of all peripherals, Initializes the Flash interface and the Systick
   HAL_Init();

   platformHW_Init();

   iprintf("\r\nStarting... (v%d | #0x%x | Built "__DATE__":"__TIME__")\r\n", FW_VERSION, bid_GetID());

   initAnimationStack();

   //display the FW version
   VersionToLEDs();
   HAL_Delay(1000);  //delay in MS

   //start the random animation
   baf_startAnimation(&AnimRGBFade, BAF_ASTART_IMMEDIATE);

   iprintf("Setting up RC5 encode/decode...");
   RC5_Encode_Init();
   RC5_Decode_Init();
   iprintf("ok\r\n");

   int cnt = 0;
   uint8_t b = 0;
   RC5_Frame_TypeDef rcf;
   while (1)
   {
      if(RC5_Decode(&rcf)) {
         iprintf("Addr   %d\r\n", rcf.Address);
         iprintf("Comd   %d\r\n", rcf.Command);
         iprintf("Field  %d\r\n", rcf.FieldBit);
         iprintf("Toggle %d\r\n", rcf.ToggleBit);
         iprintf("\r\n");

         led_SetChannel(0, COLOR_HSV_BLACK);

         for (i = 0; i < 1000000; i++);
      }

      if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
         led_SetChannel(1, COLOR_HSV_WHITE);
      }
      else {
         //we are setting a Hue
         led_SetChannel(1, COLOR_HSV_BLACK);
      }

      // spend time
      for (i = 0; i < 1000000; i++);

      if(b > 5) {
         if(cnt % 2) {
            RC5_DecodeDisable();
         }

         //addr, instruc, ctrl
         //encoded as 0x0A23
         //encoded as 0x35DC inverted (as IR RX'd)
         RC5_Encode_SendFrame(4, 23, RC5_Ctrl_Reset);
         b = 0;

         if(cnt % 2) {
            //spin until send is done, then enable RX again
            while(RC5_Encode_IsSending()) {}

            RC5_DecodeEnable();
         }

         led_SetChannel(0, COLOR_HSV_BLACK);
      }
      b++;
      cnt++;
   }

   //TODO track a systime (from systick?)
   // pump the animation frameworks
   baf_giveTime(0, NULL);
   yabi_giveTime(0);
}

/*
 * Wire up the animation framework. It's composed of two parts:
 * BAF - High level triggering to set LEDs to certain values at a timer interval.
 * YABI - Interpolates between those points to create dank RGB fading action.
 *
 * The animation objects must be statically allocated, so ours is statically allocated
 *    up at the top of the file.
 */
static void initAnimationStack(void) {
   baf_Error bres;

   struct baf_Config bc = {
      .rngCB               = bafRNGCB,
      .animationStartCB    = bafAnimStartCB,
      .animationStopCB     = bafAnimStopCB,
      .setChannelGroupCB   = bafChanGroupSetCB,
   };

   // setup the interpolator and LED HW
   led_Init();

   // seed the PRNG from the kinda unique board ID
   srand(bid_GetID());

   // setup the animation framework
   bres = baf_init(&bc);
   if(bres != BAF_OK) {
      iprintf("BAF init returned %d\n", bres);
      return;
   }
}

// a crummy RNG, but we don't really care
static uint32_t bafRNGCB(uint32_t range) {
   return rand() % range;
}

// shim to connect BAF's channel group setting API to YABI's one-at-a-time API
static void bafChanGroupSetCB(struct baf_ChannelSetting const * const channels, baf_ChannelValue* const values, uint32_t num) {
   for(int i = 0; i < num; i++) {
      //FIXME rm
      iprintf("\tSet Chan #%d to %u in %ums\n", channels[i].id, values[i], channels[i].transitionTimeMS);

      led_SetSubChannel(channels[i].id, values[i], channels[i].transitionTimeMS);
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

/*
 * Write this unit's SW version to the LEDs once.
 */
static void VersionToLEDs(void) {
   struct color_ColorHSV c = {0, 0, 0};

   //unpack each bit, and set the Blue LED channel to it
   uint16_t mask = 0x01;
   for(int i = 0; i < 10; i++) {
      // set the channel to 100 counts if the bit is set, 0 otherwise
      c.v = (mask & FW_VERSION) ? 100 : 0;

      led_SetChannel(i, c);

      mask <<= 1;
   }

   yabi_giveTime(1);
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
   /* USER CODE BEGIN 6 */
   /* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
   /* USER CODE END 6 */

}

#endif
