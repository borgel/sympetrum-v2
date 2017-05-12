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

#include "rc5_encode.h"
#include "rc5_decode.h"

#include <string.h>

static void VersionToLEDs(void);

int main(void)
{
   int i;

   // Reset of all peripherals, Initializes the Flash interface and the Systick
   HAL_Init();

   platformHW_Init();

   iprintf("\r\nStarting... (v%d | #0x%x | Built "__DATE__":"__TIME__")\r\n", FW_VERSION, bid_GetID());

   led_Init();

   //display the FW version
   VersionToLEDs();
   HAL_Delay(1000);  //delay in MS

   //FIXME rm
   struct color_ColorRGB c = {.r = 0, .g = 0, .b = 0};
   led_SetChannel(0, c);
   led_SetChannel(1, c);
   led_UpdateChannels();

   iprintf("Setting up RC5 encode/decode...");
   RC5_Encode_Init();
   RC5_Decode_Init();
   iprintf("ok\r\n");

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

         struct color_ColorRGB c = {.r = 100, .g = 0, .b = 200};
         led_SetChannel(0, c);
         led_UpdateChannels();

         for (i = 0; i < 1000000; i++);
      }

      if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
         struct color_ColorRGB cc = {.r = 0, .g = 200, .b = 0};
         led_SetChannel(1, cc);
         led_UpdateChannels();
      }
      else {
         struct color_ColorRGB cc = {.r = 0, .g = 0, .b = 0};
         led_SetChannel(1, cc);
         led_UpdateChannels();
      }

      // spend time
      for (i = 0; i < 1000000; i++);

      if(b > 5) {
         //addr, instruc, ctrl
         //encoded as 0x0A23
         //encoded as 0x35DC inverted (as IR RX'd)
         RC5_Encode_SendFrame(4, 23, RC5_Ctrl_Reset);
         b = 0;

         struct color_ColorRGB cc = {.r = 200, .g = 0, .b = 0};
         led_SetChannel(0, cc);
         led_UpdateChannels();
      }
      b++;
   }
}

/*
 * Write this unit's SW version to the LEDs once.
 */
static void VersionToLEDs(void) {
   struct color_ColorRGB c = {.r = 0, .g = 0, .b = 0};
   //unpack each bit, and set the Blue LED channel to it
   uint16_t mask = 0x01;
   for(int i = 0; i < 10; i++) {
      c.b = (mask & FW_VERSION) ? 100 : 0;
      led_SetChannel(i, c);

      mask <<= 1;
   }
   led_UpdateChannels();
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
