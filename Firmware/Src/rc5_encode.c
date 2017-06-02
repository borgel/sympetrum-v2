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

//was 64? for a 14 bit frame?
// real length is 32, X2 for Manchester Encoding
//FIXME +2 for 2 start bits?
//'real length' was 14 for RC5, which IS it's real length. but it already includes 2 start bits in the message format
static uint8_t const RC5_GlobalFrameLength = 32;

//FIXME use proper union
static uint32_t RC5_FrameBinaryFormat = 0;

// offset in packet we are sending
static uint8_t RC5_ManchesterOffset;
static uint8_t RC5_FrameBinaryOffset;
//the curent 2 bit 'Manchester Bit' in progress
static uint8_t RC5_ManchesterByte = 0x0;

//static uint32_t RC5_FrameManchestarFormat = 0;
static uint8_t Send_Operation_Ready = 0;
__IO uint8_t Send_Operation_Completed = 1;
static uint8_t BitsSent_Counter = 0;

//FIXME encapsulate this
//not static so IT can see it
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

//static uint16_t RC5_BinFrameGeneration(uint8_t RC5_Address, uint8_t RC5_Instruction, RC5_Ctrl_TypeDef RC5_Ctrl);
static uint8_t RC5_GetNextManchesterBit(void);
static void TIM17_Init(void);
static void TIM16_Init(void);

//FIXME rm?
static void Error_Handler(void) {}

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

   /* Generate a binary format of the Frame */
   //frameBinaryFormat = RC5_BinFrameGeneration(RC5_Address, RC5_Instruction, RC5_Ctrl);
   // it's already in binary format, just send it
   RC5_FrameBinaryFormat = rawMessage;

   /* Generate a Manchester format of the Frame */
   //RC5_FrameManchestarFormat = RC5_ManchesterConvert(frameBinaryFormat);

   //reset the message objects
   BitsSent_Counter = 0;
   RC5_ManchesterByte = RC5HIGHSTATE;
   RC5_ManchesterOffset = RC5_GlobalFrameLength * 2;
   RC5_FrameBinaryOffset = RC5_GlobalFrameLength;

   /* Set the Send operation Ready flag to indicate that the frame is ready to be sent */
   Send_Operation_Ready = 1;
   Send_Operation_Completed = 0;

   //FIXME re-enable
   //start the bit clock. Each edge it will send data on its own
   res = HAL_TIM_Base_Start_IT(&htim16);
   if(res != HAL_OK) {
      iprintf("Failed to start TIM16 CH1 to send\r\n");
   }

   return;

   //FIXME rm
   //HAL_TIM_Base_Stop_IT(&htim17);

   //FIXME rm
   iprintf("Send started. Binary msg = 0x%x\n", RC5_FrameBinaryFormat);

   //FIXME rm
   //return;

   //while(!Send_Operation_Completed) {
   int b;
   int i;
   for(i = 0; i < RC5_GlobalFrameLength; i++) {
      b = (RC5_FrameBinaryFormat & (1 << i)) ? 1 : 0;
      iprintf("%d", b);
      iprintf("[%d",  RC5_GetNextManchesterBit());
      iprintf("%d] ", RC5_GetNextManchesterBit());
   }
   iprintf("\r\n");

   return;

   for(i = 0; i < RC5_GlobalFrameLength * 2; i++) {
      //TODO run entire pipe?
      iprintf("%d", RC5_GetNextManchesterBit());
   }
   iprintf(" (that was %d)\n", i);
}

/**
 * Send the Manchester encoded data.
 * Called from the timer's IT!
 * */
void RC5_Encode_SignalGenerate(void)
{
   uint8_t bit_msg = 0;

   //if((Send_Operation_Ready == 1) && (BitsSent_Counter <= ((RC5_GlobalFrameLength * 2) + 2)))
   if((Send_Operation_Ready == 1) && (RC5_FrameBinaryOffset > 0))
   //if((Send_Operation_Ready == 1) && (BitsSent_Counter <= RC5_FrameBinaryOffset > 0))
   {
      //FIXME update to use a bool
      Send_Operation_Completed = 0x00;

      //bit_msg = (uint8_t)((RC5_FrameManchestarFormat >> BitsSent_Counter)& 1);
      //this is a HIGH or LOW. Manchester encoding happens inside
      bit_msg = RC5_GetNextManchesterBit() & 0x1;

      if (bit_msg == 1)
      {
         //High
         //enable the data out clock
         HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

         //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
      }
      else
      {
         //Low
         //FIXME, play out a GPIO for testing
         HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);

         //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
      }
      BitsSent_Counter++;

      //restart timer to count to next bit edge
      //FIXME needed? doesn't seem to be
      //HAL_TIM_Base_Start_IT(&htim16);
   }
   else
   {
      //FIXME rm
      iprintf("D manoff = %d (want just past 0) FBO = %d (want 0)\n", RC5_ManchesterOffset, RC5_FrameBinaryOffset);

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
   /*
   uint8_t i=0;
   uint16_t Mask = 1;
   uint16_t bit_format = 0;
   uint32_t ConvertedMsg =0;

//static uint8_t RC5_RealFrameLength = 14;
   for (i=0; i < RC5_RealFrameLength; i++)
   {
      bit_format =((((uint16_t)(RC5_BinaryFrameFormat))>>i)& Mask)<<i;
      ConvertedMsg = ConvertedMsg << 2;

      if(bit_format != 0 ) // Manchester 1 -|_
      {
         ConvertedMsg |= RC5HIGHSTATE;
      }
      else // Manchester 0 _|-
      {
         ConvertedMsg |= RC5LOWSTATE;
      }
   }
   return (ConvertedMsg);
   */

   //figure out if we need to start a new Manchester bit (if even count)
   //if so, calculate it
   //look at offset to see if we send the first or second segment of the Manchester bit

   //FIXME rm
   bool f = false;
   uint8_t bit;

   //FIXME if this is bit 0, don't send it! send an RC5HIGH instead
   //FIXME is this right?
   //if(RC5_ManchesterOffset > 1+(RC5_GlobalFrameLength * 3)) {
   /*
   if(RC5_ManchesterOffset > 66) {
      Send_Operation_Ready = 0;

      return 0;
   }
   else if(RC5_ManchesterOffset == 0) {
   */
   if(RC5_ManchesterOffset == (RC5_GlobalFrameLength * 2) + 2) {
      RC5_ManchesterByte = RC5HIGHSTATE;
      //RC5_ManchesterByte = RC5LOWSTATE;
      iprintf("s1");
   }
   else if(RC5_ManchesterOffset == (RC5_GlobalFrameLength * 2) - 0) {
      RC5_ManchesterByte = RC5HIGHSTATE;
      //RC5_ManchesterByte = RC5LOWSTATE;
      iprintf("s2");
   }

   // if we need to calculate a new 'Manchester Bit'
   //FIXME make this not a global
   else if(RC5_ManchesterOffset % 2 == 0) {
      //iprintf("(R %d)", RC5_FrameBinaryFormat & (1 << RC5_FrameBinaryOffset));
      //if(RC5_FrameBinaryFormat & (1 << RC5_FrameBinaryOffset))
      if(RC5_FrameBinaryFormat & (1 << RC5_FrameBinaryOffset))
      {
         //RC5_ManchesterByte = RC5HIGHSTATE;
         RC5_ManchesterByte = RC5LOWSTATE;

         //FIXME rm
         iprintf("1");
      }
      else // Manchester 0 _|-
      {
         //RC5_ManchesterByte = RC5LOWSTATE;
         RC5_ManchesterByte = RC5HIGHSTATE;

         //FIXME rm
         iprintf("0");
      }

      //FIXME rm
      if(RC5_FrameBinaryOffset == 0) {
         f = true;
      }

      RC5_FrameBinaryOffset--;
   }

   //FIXME rm
   //iprintf("%x[%d](%d) ", RC5_ManchesterByte, RC5_ManchesterOffset % 2, RC5_FrameBinaryFormat & 0x1);

   //redundant to make the logic clear
   if(RC5_ManchesterOffset % 2 == 0) {
      bit = RC5_ManchesterByte & (1 << 0);
   }
   else {
      bit = RC5_ManchesterByte & (1 << 1);

      //FIXME rm
      //iprintf("\n");
   }

   RC5_ManchesterOffset--;

   if(f) {
      iprintf("\r\n");
   }
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
      Error_Handler();
   }

   if (HAL_TIM_PWM_Init(&htim16) != HAL_OK)
   {
      Error_Handler();
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
      Error_Handler();
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
      Error_Handler();
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
      Error_Handler();
   }

   if (HAL_TIM_PWM_Init(&htim17) != HAL_OK)
   {
      Error_Handler();
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
      Error_Handler();
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
      Error_Handler();
   }
}

