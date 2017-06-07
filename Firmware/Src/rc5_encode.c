/*
 */
#include "rc5_encode.h"
#include "platform_hw.h"
#include "iprintf.h"

#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_tim.h"

#include <stdint.h>

#define  RC5HIGHSTATE     ((uint8_t )0x02)   /* RC5 high level definition, 10 */
#define  RC5LOWSTATE      ((uint8_t )0x01)   /* RC5 low level definition   01 */

//'real length' was 14 for RC5, which IS it's real length. but it already includes 2 start bits in the message format
static uint8_t const RC5_GlobalFrameLength = 32;

//FIXME use proper union
static uint32_t RC5_FrameBinaryFormat;

// offset in packet we are sending
static uint8_t RC5_ManchesterOffset;
static uint8_t RC5_FrameBinaryOffset;

// must persist between calls to generate manchester frame
//the curent 2 bit 'Manchester Bit' in progress
static uint8_t RC5_ManchesterByte = 0x0;

//static uint32_t RC5_FrameManchestarFormat = 0;
static uint8_t Send_Operation_Ready = 0;
//FIXME update to use a bool
__IO uint8_t Send_Operation_Completed = 1;
static uint8_t BitsSent_Counter = 0;

//FIXME encapsulate this
//not static so IT can see it
TIM_HandleTypeDef htim16;
static TIM_HandleTypeDef htim17;

static uint8_t RC5_GetNextManchesterBit(void);
static void TIM17_Init(void);
static void TIM16_Init(void);

void RC5_Encode_Init(void)
{
   TIM17_Init();
   TIM16_Init();
}

//FIXME take a msg union like encode?
void RC5_Encode_SendFrame(uint32_t rawMessage)
{
   HAL_StatusTypeDef res;

   Send_Operation_Ready = 0;

   RC5_FrameBinaryFormat = rawMessage;

   //reset the message objects
   BitsSent_Counter = 0;
   RC5_ManchesterByte = RC5HIGHSTATE;
   RC5_ManchesterOffset = RC5_GlobalFrameLength * 2;
   RC5_FrameBinaryOffset = RC5_GlobalFrameLength - 1;

   /* Set the Send operation Ready flag to indicate that the frame is ready to be sent */
   Send_Operation_Ready = 1;
   Send_Operation_Completed = 0;

   //start the bit clock. Each edge it will send data on its own
   res = HAL_TIM_Base_Start_IT(&htim16);
   if(res != HAL_OK) {
      iprintf("Failed to start TIM16 CH1 to send\r\n");
   }
}

/**
 * Send the Manchester encoded data.
 * Called from the timer's IT!
 * */
void RC5_Encode_SignalGenerate(void)
{
   uint8_t bit_msg = 0;

   if(Send_Operation_Ready == 1)
   {
      Send_Operation_Completed = 0x00;

      bit_msg = RC5_GetNextManchesterBit() & 0x1;

      if (bit_msg == 1)
      {
         //High
         //enable the 38k data out clock
         HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
      }
      else
      {
         //Low
         HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);
      }
      BitsSent_Counter++;

      //restart timer to count to next bit edge
      //FIXME needed? doesn't seem to be
      //HAL_TIM_Base_Start_IT(&htim16);
   }
   else
   {
      HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);

      Send_Operation_Completed = 0x01;

      HAL_StatusTypeDef res = HAL_OK;

      res = HAL_TIM_Base_Stop_IT(&htim16);
      if(res != HAL_OK) {
         iprintf("Failed to stop TIM16 after sending message\r\n");
      }

      //force TIM17's output low so it never accidentally idles high after sending
      //(which would burn lots of power)
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

      Send_Operation_Ready = 0;
      BitsSent_Counter = 0;
   }
}

/**
 * Get the next bit of the frame, in Manchester encoded format.
 *
 * Returns 0 for low, !0 for high
 */
static uint8_t RC5_GetNextManchesterBit(void) {
   uint8_t bit;

   //send the start sequence
   if(RC5_ManchesterOffset == (RC5_GlobalFrameLength * 2) + 0) {
      RC5_ManchesterByte = RC5LOWSTATE;
   }
   // if we need to calculate a new 'Manchester Bit'
   else if(RC5_ManchesterOffset % 2 == 0) {
      if(RC5_FrameBinaryFormat & (1 << RC5_FrameBinaryOffset))
      {
         RC5_ManchesterByte = RC5LOWSTATE;
      }
      else // Manchester 0 _|-
      {
         RC5_ManchesterByte = RC5HIGHSTATE;
      }

      RC5_FrameBinaryOffset--;
   }

   //redundant to make the logic clear
   if(RC5_ManchesterOffset % 2 == 0) {
      bit = RC5_ManchesterByte & (1 << 0);
   }
   else {
      bit = RC5_ManchesterByte & (1 << 1);
   }

   if(RC5_ManchesterOffset == 0) {
      Send_Operation_Ready = 0;
   }

   RC5_ManchesterOffset--;

   return bit == 0;
}

bool RC5_Encode_IsSending(void) {
   return (Send_Operation_Completed == 0x00);
}

/* TIM16 init function */
static void TIM16_Init(void)
{
   TIM_OC_InitTypeDef sConfigOC;
   TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

   htim16.Instance = TIM16;
   htim16.Init.Prescaler = 0;
   htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim16.Init.Period = 42627;
   htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   htim16.Init.RepetitionCounter = 0;
   htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
   if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
   {
      iprintf("Error\r\n");
      return;
   }

   if (HAL_TIM_PWM_Init(&htim16) != HAL_OK)
   {
      iprintf("Error\r\n");
      return;
   }

   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 42627;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
   sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
   if (HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
   {
      iprintf("Error\r\n");
      return;
   }

   sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
   sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
   sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
   sBreakDeadTimeConfig.DeadTime = 0;
   sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
   sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
   sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
   if (HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig) != HAL_OK)
   {
      iprintf("Error\r\n");
      return;
   }
}

/* TIM17 init function */
static void TIM17_Init(void)
{
   TIM_OC_InitTypeDef sConfigOC;
   TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

   htim17.Instance = TIM17;
   htim17.Init.Prescaler = 0;
   htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim17.Init.Period = 1333;
   htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   htim17.Init.RepetitionCounter = 0;
   htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
   if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
   {
      iprintf("Error\r\n");
      return;
   }

   if (HAL_TIM_PWM_Init(&htim17) != HAL_OK)
   {
      iprintf("Error\r\n");
      return;
   }

   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 333;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
   sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
   if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
   {
      iprintf("Error\r\n");
      return;
   }

   sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
   sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
   sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
   sBreakDeadTimeConfig.DeadTime = 0;
   sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
   sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
   sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
   if (HAL_TIMEx_ConfigBreakDeadTime(&htim17, &sBreakDeadTimeConfig) != HAL_OK)
   {
      iprintf("Error\r\n");
      return;
   }
}

