#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_tim.h"


void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/
  /* SVC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==SPI1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    /**SPI1 GPIO Configuration
    PB3     ------> SPI1_SCK
    PB5     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PB3     ------> SPI1_SCK
    PB5     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5);
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
  }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   //Bring up IR Decode peripherals
   if(htim_base->Instance==TIM3)
   {
      /* Peripheral clock enable */
      __HAL_RCC_TIM3_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();

      GPIO_InitStruct.Pin = GPIO_PIN_6;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      /* Peripheral interrupt init */
      HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(TIM3_IRQn);
   }

   //Bring up IR Encode Envelope peripherals
   else if(htim_base->Instance==TIM16)
   {
      /* Peripheral clock enable */
      __HAL_RCC_TIM16_CLK_ENABLE();

      /* Peripheral interrupt init */
      HAL_NVIC_SetPriority(TIM16_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(TIM16_IRQn);
   }
   //Bring up IR Encode Carrier peripherals
   else if(htim_base->Instance==TIM17)
   {
      /* Peripheral clock enable */
      __HAL_RCC_TIM17_CLK_ENABLE();

      /* Peripheral interrupt init */
      //HAL_NVIC_SetPriority(TIM17_IRQn, 0, 0);
      //HAL_NVIC_EnableIRQ(TIM17_IRQn);

      __HAL_RCC_GPIOA_CLK_ENABLE();
      GPIO_InitStruct.Pin = GPIO_PIN_7;
      //GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      //GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      //FIXME ENABLE! or timer won't be connected anymore...
      //GPIO_InitStruct.Alternate = GPIO_AF5_TIM17;
      //GPIO_InitStruct.Alternate = GPIO_AF5_TIM17;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
   }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

   if(htim_base->Instance==TIM3)
   {
      /* Peripheral clock disable */
      __HAL_RCC_TIM3_CLK_DISABLE();

      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);

      /* Peripheral interrupt DeInit*/
      HAL_NVIC_DisableIRQ(TIM3_IRQn);
   }
   else if(htim_base->Instance==TIM16)
   {
      /* Peripheral clock enable */
      __HAL_RCC_TIM16_CLK_DISABLE();

      HAL_NVIC_DisableIRQ(TIM16_IRQn);
   }
   //Bring up IR Encode Carrier peripherals
   else if(htim_base->Instance==TIM17) {
      __HAL_RCC_TIM17_CLK_DISABLE();

      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);

      HAL_NVIC_DisableIRQ(TIM17_IRQn);
   }
}

