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
/**
  * @}
  */

/** @defgroup RC5_ENCODE_Private_FunctionPrototypes
  * @{
  */
static uint16_t RC5_BinFrameGeneration(uint8_t RC5_Address, uint8_t RC5_Instruction, RC5_Ctrl_TypeDef RC5_Ctrl);
static uint32_t RC5_ManchesterConvert(uint16_t RC5_BinaryFrameFormat);

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
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* TIM16 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
  
  /* TIM17 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
  
  /* Pin configuration: input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_0);

  /* DeInit TIM17 */
  TIM_DeInit(TIM17);

  /* Elementary period 888us */
  /* Time base configuration for timer 2 */
  TIM_TimeBaseStructure.TIM_Period = 1333;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
  
  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM17, 0, TIM_PSCReloadMode_Immediate);
  
  /* Output Compare Timing Mode configuration: Channel 1N */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1333/4; /* Set duty cycle to 25% to be compatible with RC5 specification */
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM17, &TIM_OCInitStructure);
  
  /* Timer17 preload enable */
  TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable);
  
  /* Timer 17 Enable */
  TIM_Cmd(TIM17, ENABLE);
  
  /* Enable the TIM16 channel1 output to be connected internly to the IRTIM */
  TIM_CtrlPWMOutputs(TIM17, ENABLE);
  
  /* DeInit TIM16 */
  TIM_DeInit(TIM16);

  /* Time Base = 36Khz */
  /* Time Base configuration for timer 16 */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 42627;//666;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
  
  /* Duty Cycle = 25% */
  /* Channel 1 Configuration in Timing mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 42627;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM16, &TIM_OCInitStructure);
   
  /* Enable the TIM17 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* TIM16 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM16, ENABLE);
  
  /* TIM IT Disable */
  TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
  
  /* TIM Disable */
  TIM_Cmd(TIM16, DISABLE);
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
  uint16_t RC5_FrameBinaryFormat = 0;
  
  /* Generate a binary format of the Frame */
  RC5_FrameBinaryFormat = RC5_BinFrameGeneration(RC5_Address, RC5_Instruction, RC5_Ctrl);
  
  /* Generate a Manchester format of the Frame */
  RC5_FrameManchestarFormat = RC5_ManchesterConvert(RC5_FrameBinaryFormat);
  
  /* Set the Send operation Ready flag to indicate that the frame is ready to be sent */
  Send_Operation_Ready = 1;
  
  /* TIM IT Enable */
  TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
  /* Enable all Interrupt */
  TIM_Cmd(TIM16, ENABLE);
}

/**
  * @brief  Send by hardware Manchester Format RC5 Frame.
  * @param  RC5_BinaryFrameFormat: the RC5 frame in binary format.
  * @retval Noe
  */
void RC5_Encode_SignalGenerate(uint32_t RC5_ManchestarFrameFormat)
{
  uint8_t bit_msg = 0;
  
  if((Send_Operation_Ready == 1) && (BitsSent_Counter <= (RC5_GlobalFrameLength * 2)))
  {
    Send_Operation_Completed = 0x00;
    bit_msg = (uint8_t)((RC5_ManchestarFrameFormat >> BitsSent_Counter)& 1);
    
    if (bit_msg== 1)
    {
      TIM_ForcedOC1Config(TIM16, TIM_ForcedAction_Active);
    }
    else
    {
      TIM_ForcedOC1Config(TIM16, TIM_ForcedAction_InActive);
    }
    BitsSent_Counter++;
  }
  else
  {
    Send_Operation_Completed = 0x01;
    /* TIM IT Disable */
    TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
    TIM_Cmd(TIM16, DISABLE);
    Send_Operation_Ready = 0;
    BitsSent_Counter = 0;
    TIM_ForcedOC1Config(TIM16, TIM_ForcedAction_InActive);
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
