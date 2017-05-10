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


UART_HandleTypeDef huart1;

static void VersionToLEDs(void);

void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);


int main(void)
{
   int i;

   // Reset of all peripherals, Initializes the Flash interface and the Systick
   HAL_Init();

   // Configure the system clock
   SystemClock_Config();

   // Initialize all configured peripherals
   MX_GPIO_Init();
   MX_USART1_UART_Init();

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

/** System Clock Configuration
 */
void SystemClock_Config(void)
{

   RCC_OscInitTypeDef RCC_OscInitStruct;
   RCC_ClkInitTypeDef RCC_ClkInitStruct;

   /**Initializes the CPU, AHB and APB busses clocks 
    */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
   RCC_OscInitStruct.HSICalibrationValue = 16;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
   RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
   RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
      Error_Handler();
   }

   /**Initializes the CPU, AHB and APB busses clocks 
    */
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
      |RCC_CLOCKTYPE_PCLK1;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
   {
      Error_Handler();
   }

   /**Configure the Systick interrupt time 
    */
   HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

   /**Configure the Systick 
    */
   HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

   /* SysTick_IRQn interrupt configuration */
   HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

   huart1.Instance = USART1;
   huart1.Init.BaudRate = 115200;
   huart1.Init.WordLength = UART_WORDLENGTH_8B;
   huart1.Init.StopBits = UART_STOPBITS_1;
   huart1.Init.Parity = UART_PARITY_NONE;
   huart1.Init.Mode = UART_MODE_TX_RX;
   huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart1.Init.OverSampling = UART_OVERSAMPLING_16;
   huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
   huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
   if (HAL_UART_Init(&huart1) != HAL_OK)
   {
      Error_Handler();
   }
}

/** Configure pins as 
 * Analog 
 * Input 
 * Output
 * EVENT_OUT
 * EXTI
 */
static void MX_GPIO_Init(void)
{

   GPIO_InitTypeDef GPIO_InitStruct;

   /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();

   // setup button
   GPIO_InitStruct.Pin = USER_BUTTON_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(USER_BUTTON_PORT, &GPIO_InitStruct);

   //setup button vector
   HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
   //TODO add error logging
   iprintf("\r\n\r\n");
   iprintf("ERROR!");
   iprintf("\r\n\r\n");
   while(1) { }
   /* USER CODE END Error_Handler */ 
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
