/**
  ******************************************************************************
  * @file    rc5_encode.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file provides all the sony rc5 encode firmware functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/        
#include "main.h"
#include "rc5_encode.h"
#include "ir_decode.h"
#include "iprintf.h"

#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_tim.h"

#include <stdint.h>

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup RC5_ENCODE
  * @brief RC5_ENCODE driver modules
  * @{
  */

/** @defgroup RC5_ENCODE_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup RC5_ENCODE_Private_Defines
  * @{
  */
#define  RC5HIGHSTATE     ((uint8_t )0x02)   /* RC5 high level definition*/
#define  RC5LOWSTATE      ((uint8_t )0x01)   /* RC5 low level definition*/
/**
  * @}  
  */


/** @defgroup RC5_ENCODE_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RC5_ENCODE_Private_Variables
  * @{
  */
uint8_t RC5_RealFrameLength = 14;
uint8_t RC5_GlobalFrameLength = 64;
uint16_t RC5_FrameBinaryFormat = 0;
uint32_t RC5_FrameManchestarFormat = 0;
uint8_t Send_Operation_Ready = 0;
__IO uint8_t Send_Operation_Completed = 1;
uint8_t BitsSent_Counter = 0;

uint8_t AddressIndex = 0;
uint8_t InstructionIndex = 0;
RC5_Ctrl_TypeDef RC5_Ctrl1 = RC5_Ctrl_Reset;
RC5_Ctrl_TypeDef RC5_Ctrl2 = RC5_Ctrl_Reset;
extern uint8_t* rc5_Commands[];
extern uint8_t* rc5_devices[];
extern __IO uint8_t RFDemoStatus; 

//FIXME pass in?
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
/**
  * @}
  */

/** @defgroup RC5_ENCODE_Private_FunctionPrototypes
  * @{
  */
static uint16_t RC5_BinFrameGeneration(uint8_t RC5_Address, uint8_t RC5_Instruction, RC5_Ctrl_TypeDef RC5_Ctrl);
static uint32_t RC5_ManchesterConvert(uint16_t RC5_BinaryFrameFormat);
static void MX_TIM17_Init(void);
static void MX_TIM16_Init(void);

//FIXME rm?
static void Error_Handler(void) {}

/**
  * @}
  */

/** @defgroup RC5_ENCODE_Private_Functions
  * @{
  */

/**
  * @brief  RCR receiver demo exec.
  * @param  None
  * @retval None
  */
//FIXME rm, this isn't really useful now
void Menu_RC5_Encode_Func(void)
{
  RC5_Encode_Init();

  AddressIndex = 0;
  InstructionIndex = 0;


  //LCD_DisplayStringLine(LCD_LINE_6, rc5_Commands[InstructionIndex]);
  //LCD_DisplayStringLine(LCD_LINE_7, rc5_devices[AddressIndex]);

  RC5_Encode_SendFrame(AddressIndex, InstructionIndex, RC5_Ctrl1);
}

/**
  * @brief  Init Hardware (IPs used) for RC5 generation
  * @param  None
  * @retval None
  */
void RC5_Encode_Init(void)
{

   MX_TIM17_Init();
   MX_TIM16_Init();

   /*
   //FIXME TODO disable each?
   HAL_StatusTypeDef res;

   //res = HAL_TIM_PWM_Start(&htim17, 1);
   //2nd param is u32 channel
   //res = HAL_TIM_PWM_Stop(&htim17, 1);
   if(res != HAL_OK) {
      iprintf("Failed to stop TIM17 CH1 after init\r\n");
   }
   res = HAL_TIM_PWM_Stop(&htim16, 1);
   if(res != HAL_OK) {
      iprintf("Failed to stop TIM16 CH1 after init\r\n");
   }
   */
}

/**
  * @brief  Generate and Send the RC5 frame.
  * @param  RC5_Address: the RC5 Device destination 
  * @param  RC5_Instruction: the RC5 command instruction 
  * @param  RC5_Ctrl: the RC5 Control bit.
  * @retval None
  */
void RC5_Encode_SendFrame(uint8_t RC5_Address, uint8_t RC5_Instruction, RC5_Ctrl_TypeDef RC5_Ctrl)
{
  HAL_StatusTypeDef res;
  uint16_t RC5_FrameBinaryFormat = 0;

  /* Generate a binary format of the Frame */
  RC5_FrameBinaryFormat = RC5_BinFrameGeneration(RC5_Address, RC5_Instruction, RC5_Ctrl);

  /* Generate a Manchester format of the Frame */
  RC5_FrameManchestarFormat = RC5_ManchesterConvert(RC5_FrameBinaryFormat);

  /* Set the Send operation Ready flag to indicate that the frame is ready to be sent */
  Send_Operation_Ready = 1;

  //start the bit clock. Each edge it will send data on its own
  res = HAL_TIM_Base_Start_IT(&htim16);
  if(res != HAL_OK) {
     iprintf("Failed to start TIM16 CH1 to send\r\n");
  }
}

/**
  * @brief  Send by hardware Manchester Format RC5 Frame.
  * @param  RC5_BinaryFrameFormat: the RC5 frame in binary format.
  * @retval Noe
  */
//FIXME used to take in "RC5_ManchestarFrameFormat, not sure why. Replaced
//with using RC5_FrameManchestarFormat directly.
void RC5_Encode_SignalGenerate(void)
{
  uint8_t bit_msg = 0;

  if((Send_Operation_Ready == 1) && (BitsSent_Counter <= (RC5_GlobalFrameLength * 2)))
  {
    Send_Operation_Completed = 0x00;
    bit_msg = (uint8_t)((RC5_FrameManchestarFormat >> BitsSent_Counter)& 1);

    if (bit_msg== 1)
    {
       //enable the data out clock
       HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

       /*
       //FIXME rm, play out a GPIO for testing
       if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin)) {
         HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
       }
       */
       if(GPIO_PIN_SET == HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin)) {
          HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
       }
    }
    else
    {
       //FIXME rm, play out a GPIO for testing
       HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);

       /*
       //FIXME rm
       if(GPIO_PIN_SET == HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin)) {
          HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
       }
       */
       if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin)) {
         HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
       }
    }
    BitsSent_Counter++;

    //restart timer to count to next bit edge
    HAL_TIM_Base_Start_IT(&htim16);
  }
  else
  {
    Send_Operation_Completed = 0x01;

   HAL_StatusTypeDef res;

   res = HAL_TIM_Base_Stop_IT(&htim16);
   if(res != HAL_OK) {
      iprintf("Failed to stop TIM16 after sending message\r\n");
   }

   //force TIM17's output low so it never accidentally idles high after sending
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

   Send_Operation_Ready = 0;
   BitsSent_Counter = 0;
  }
}
/**
  * @brief  Generate the binary format of the RC5 frame.
  * @param  RC5_Address: Select the device adress.
  * @param  RC5_Instruction: Select the device instruction.
  * @param  RC5_Ctrl: Select the device control bit status.
  * @retval Binary format of the RC5 Frame.
  */
static uint16_t RC5_BinFrameGeneration(uint8_t RC5_Address, uint8_t RC5_Instruction, RC5_Ctrl_TypeDef RC5_Ctrl)
{
  uint16_t star1 = 0x2000;
  uint16_t star2 = 0x1000;
  uint16_t addr = 0;
  
  while(Send_Operation_Completed == 0x00) 
  { 
  }

  /* Check if Instruction is 128-bit length */
  if(RC5_Instruction >= 64)
  {
    /* Reset field bit: command is 7-bit length */
    star2 = 0;
    /* Keep the lowest 6 bits of the command */
    RC5_Instruction &= 0x003F;
  }
  else /* Instruction is 64-bit length */
  {
    /* Set field bit: command is 6-bit length */
    star2 = 0x1000;
  }

  Send_Operation_Ready = 0;
  RC5_FrameManchestarFormat = 0;
  RC5_FrameBinaryFormat=0;
  addr = ((uint16_t)(RC5_Address))<<6;
  RC5_FrameBinaryFormat =  (star1)|(star2)|(RC5_Ctrl)|(addr)|(RC5_Instruction);
  return (RC5_FrameBinaryFormat);
}

/**
  * @brief  Convert the RC5 frame from binary to Manchester Format.
  * @param  RC5_BinaryFrameFormat : the RC5 frame in binary format.
  * @retval The RC5 frame in Manchester format.
  */
static uint32_t RC5_ManchesterConvert(uint16_t RC5_BinaryFrameFormat)
{
  uint8_t i=0;
  uint16_t Mask = 1;
  uint16_t bit_format = 0;
  uint32_t ConvertedMsg =0;
  
  for (i=0; i < RC5_RealFrameLength; i++)
  {
    bit_format =((((uint16_t)(RC5_BinaryFrameFormat))>>i)& Mask)<<i;
    ConvertedMsg = ConvertedMsg << 2;
    
    if(bit_format != 0 ) /* Manchester 1 -|_  */
    {
      ConvertedMsg |= RC5HIGHSTATE;
    }
    else /* Manchester 0 _|-  */
    {
      ConvertedMsg |= RC5LOWSTATE;
    }
  }
  return (ConvertedMsg);
}

/* TIM16 init function */
static void MX_TIM16_Init(void)
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

  /*
  //for reference, this is how to start the timer with interrupts
  if(HAL_TIM_Base_Start_IT(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
   */
}

/* TIM17 init function */
static void MX_TIM17_Init(void)
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
  /*
  //for reference, this is how to start the PWM
  if(HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
   */
}

/**
* @}
*/

/**
  * @}
  */

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
