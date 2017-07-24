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

#include "pattern.h"

#include <string.h>
#include <stdlib.h>

static const int VersionFadeSpeedMS = 3000;

static void VersionToLEDs(void);

int main(void)
{
   // Reset of all peripherals, Initializes the Flash interface and the Systick
   HAL_Init();

   platformHW_Init();

   iprintf("\r\nStarting... (v%d | #0x%x / 0x%x | Built "__DATE__":"__TIME__")\r\n", FW_VERSION, bid_GetID(), bid_GetIDCrc());

   // seed the PRNG from the kinda unique board ID
   srand(bid_GetID());

   // setup the entire LED framework (w/ animation)
   led_Init();

   // Display the FW version on the LEDs
   VersionToLEDs();

   pattern_Init();

   //FIXME rm
   //while(1) {}

   // FIXME rm?
   /*
   for(int i = 0; i < LED_CHAIN_LENGTH; i++) {
      led_SetChannel(i, COLOR_HSV_WHITE);
   }
   led_GiveTime(2000);
   */

   /*
   while(1)
   {
      iprintf("<<< Starting %dms >>>\r\n", HAL_GetTick());
      led_GiveTime(HAL_GetTick());
      HAL_Delay(50);
   }
    */

   while (1)
   {
      /*
      if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
         led_SetChannel(1, COLOR_HSV_WHITE);
      }
      else {
         //we are setting a Hue
         led_SetChannel(1, COLOR_HSV_BLACK);
      }
      */
      pattern_GiveTime(HAL_GetTick());
      led_GiveTime(HAL_GetTick());
   }
}

/*
 * Write this unit's SW version to the LEDs once.
 */
static void VersionToLEDs(void) {
   struct color_ColorHSV c;

   //unpack each bit, and set the Blue LED channel to it
   uint16_t mask = 0x01;
   for(int i = 0; i < LED_CHAIN_LENGTH; i++) {
      // set the channel to 100 counts if the bit is set, 0 otherwise
      c.h = (mask & FW_VERSION) ? HSV_COLOR_B : HSV_COLOR_G;
      c.s = 100;
      c.v = 100;

      led_SetChannelTimed(i, c, VersionFadeSpeedMS);

      mask <<= 1;
   }

   //FIXME rm
   iprintf("Start turn on fade...\n");

   // Fade in over 1.5 seconds
   uint32_t startTime = HAL_GetTick();
   //for(int i = 0; i < 150; i++) {
   while(HAL_GetTick() - startTime < VersionFadeSpeedMS) {
      yabi_giveTime(HAL_GetTick());
   }

   iprintf("DONE\n");
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
