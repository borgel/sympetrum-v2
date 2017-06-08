#include "platform_hw.h"
#include "iprintf.h"

#include <string.h>

// the UART used for iprintf
UART_HandleTypeDef huart1;

static uint8_t const LED_FRAME_START[4] = {0x00, 0x00, 0x00, 0x00};
static uint8_t const LED_FRAME_STOP[4]  = {0xFF, 0xFF, 0xFF, 0xFF};

#define LED_GLOB_BRIGHTNESS_MAX     0x1F
#define LED_GLOB_BRIGHTNESS_MIN     0x01
#define LED_GLOB_BRIGHTNESS         (LED_GLOB_BRIGHTNESS_MIN)

//pull in from outside
union platformHW_LEDRegister  LedRegisterStates[LED_CHAIN_LENGTH] = {
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
   {.header = 0x7, .globalBrightness = LED_GLOB_BRIGHTNESS},
};

static void SystemClock_Config(void);
static void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);


/*
 * Setup all the non specific HW in the system.
 */
bool platformHW_Init(void) {
   // Configure the system clock
   SystemClock_Config();

   // Initialize all configured peripherals
   MX_GPIO_Init();
   MX_USART1_UART_Init();

   return true;
}

void platformHW_UpdateLEDs(SPI_HandleTypeDef* spi) {
   int i;

   //TODO we have to strip the const here. That's ok, right? Read the SRC
   //TODO what is a better timeout here?
   HAL_SPI_Transmit(spi, (uint8_t*)LED_FRAME_START, sizeof(LED_FRAME_START), 10000);

   for(i = 0; i < LED_CHAIN_LENGTH; i++) {
      HAL_SPI_Transmit(spi, LedRegisterStates[i].raw, 4, 10000);
   }

   HAL_SPI_Transmit(spi, (uint8_t*)LED_FRAME_STOP, sizeof(LED_FRAME_STOP), 10000);
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

/* SPI1 init function */
bool platformHW_SpiInit(SPI_HandleTypeDef * const spi, SPI_TypeDef* spiInstance)
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

