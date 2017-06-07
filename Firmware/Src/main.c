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

#include "ir_encode.h"
#include "ir_decode.h"

#include <string.h>
#include <stdlib.h>

static void VersionToLEDs(void);


int main(void)
{
   int i;

   // Reset of all peripherals, Initializes the Flash interface and the Systick
   HAL_Init();

   platformHW_Init();

   iprintf("\r\nStarting... (v%d | #0x%x | Built "__DATE__":"__TIME__")\r\n", FW_VERSION, bid_GetID());

   // seed the PRNG from the kinda unique board ID
   srand(bid_GetID());

   // setup the entire LED framework (w/ animation)
   led_Init();

   //display the FW version
   VersionToLEDs();
   HAL_Delay(1000);  //delay in MS

   iprintf("Setting up RC5 encode/decode...");
   ir_InitEncode();
   ir_InitDecode();
   iprintf("ok\r\n");

   led_StartAnimation();

   /*
   //FIXME rm
   uint32_t time = 0;
   while(1) {
      //iprintf("<<< Starting %dms >>>\r\n", time);
      led_GiveTime(time);

      HAL_Delay(30);
      time += 30;
   }
   */

   int cnt = 0;
   uint8_t b = 0;
   RC5_Frame_TypeDef rcf;
   while (1)
   {
      if(ir_GetDecoded(&rcf)) {
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
            ir_DecodeDisable();
         }

         //ir_SendRC5(4, 23, RC5_Ctrl_Reset);
         ir_SendRaw(0x0EEF);
         b = 0;

         if(cnt % 2) {
            //spin until send is done, then enable RX again
            while(ir_IsSending()) {}

            ir_DecodeEnable();
         }

         led_SetChannel(0, COLOR_HSV_BLACK);
      }
      b++;
      cnt++;
   }

   //TODO track a systime (from systick?)
   // pump the animation frameworks
   led_GiveTime(0);
}

/*
 * Write this unit's SW version to the LEDs once.
 */
//FIXME fix color logic stuff
static void VersionToLEDs(void) {
   struct color_ColorHSV c = {.h = 0, .s = 255, .v = 0};

   //unpack each bit, and set the Blue LED channel to it
   uint16_t mask = 0x01;
   for(int i = 0; i < LED_CHAIN_LENGTH; i++) {
      // set the channel to 100 counts if the bit is set, 0 otherwise
      c.h = (mask & FW_VERSION) ? HSV_COLOR_B : HSV_COLOR_G;
      c.s = 254;
      c.v = (mask & FW_VERSION) ? 100: 0;

      led_SetChannel(i, c);

      mask <<= 1;
   }

   //FIXME rm
   //yabi_giveTime(1);
   led_GiveTime(1);
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
