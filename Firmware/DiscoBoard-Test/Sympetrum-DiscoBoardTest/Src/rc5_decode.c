/**
  ******************************************************************************
  * @file    rc5_decode.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file provides all the RC5 firmware functions.
  *   
  * 1. How to use this driver
  * -------------------------  
  *      - Call the function RC5_Init() to configure the Timer and GPIO hardware
  *        resources needed for RC5 decoding.
  *        
  *      - TIM2 Capture Compare and Update interrupts are used to decode the RC5
  *        frame, if a frame is received correctly a global variable RC5FrameReceived 
  *        will be set to inform the application.
  *          
  *      - Then the application should call the function RC5_Decode() to retrieve 
  *        the received RC5 frame.
  *                   
  * 2. Important to know
  * --------------------  
  *      - TIM2_IRQHandler  ISRs are coded within
  *        this driver.
  *        If you are using one or both Interrupts in your application you have 
  *        to proceed carefully:   
  *           - either add your application code in these ISRs
  *           - or copy the contents of these ISRs in you application code
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

/** @addtogroup RC5_DECODE
  * @brief RC5_DECODE driver modules
  * @{
  */

/** @defgroup RC5_DECODE_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup RC5_DECODE_Private_Defines
  * @{
  */
/**
  * @}  
  */


/** @defgroup RC5_DECODE_Private_Macros
  * @{
  */
/* RC5 address table */
uint8_t* rc5_devices[32] = {
        "       TV1          ",                  /*  0 */
        "       TV2          ",                  /*  1 */
        "    Video Text      ",                  /*  2 */
        "   Extension TV     ",                  /*  3 */
        "  LaserVideoPlayer  ",                  /*  4 */
        "       VCR1         ",                  /*  5 */
        "       VCR2         ",                  /*  6 */
        "      Reserved      ",                  /*  7 */
        "       Sat1         ",                  /*  8 */
        "   Extension VCR    ",                  /*  9 */
        "       Sat2         ",                  /* 10 */
        "     Reserved       ",                  /* 11 */
        "     CD Video       ",                  /* 12 */
        "      Reserved      ",                  /* 13 */
        "     CD Photo       ",                  /* 14 */
        "      Reserved      ",                  /* 15 */
        " Preampli Audio 1   ",                  /* 16 */
        "      Tuner         ",                  /* 17 */
        "  Analog Magneto    ",                  /* 18 */
        " Preampli Audio 2   ",                  /* 19 */
        "       CD           ",                  /* 20 */
        "    Rack Audio      ",                  /* 21 */
        " Audio Sat Receiver ",                  /* 22 */
        "   DDC Magneto      ",                  /* 23 */
        "     Reserved       ",                  /* 24 */
        "     Reserved       ",                  /* 25 */
        "      CDRW          ",                  /* 26 */
        "     Reserved       ",                  /* 27 */
        "     Reserved       ",                  /* 28 */
        "     Reserved       ",                  /* 29 */
        "     Reserved       ",                  /* 30 */
        "     Reserved       "                   /* 31 */  	             
       };

/* RC5 commands table*/
uint8_t* rc5_Commands[128] = {
        "       Num0         ",                                       /* 0 */
        "       Num1         ",                                       /* 1 */
        "       Num2         ",                                       /* 2 */
        "       Num3         ",                                       /* 3 */
        "       Num4         ",                                       /* 4 */
        "       Num5         ",                                       /* 5 */
        "       Num6         ",                                       /* 6 */
        "       Num7         ",                                       /* 7 */
        "       Num8         ",                                       /* 8 */
        "       Num9         ",                                       /* 9 */
        "     TV Digits      ",                                       /* 10 */
        "      TV Freq       ",                                       /* 11 */
        "     TV StandBy     ",                                       /* 12 */
        "  TV Mute On-Off    ",                                       /* 13 */
        "   TV Preference    ",                                       /* 14 */
        "    TV Display      ",                                       /* 15 */
        "    Volume Up       ",                                       /* 16 */
        "    Volume Down     ",                                       /* 17 */
        "    Brightness Up   ",                                       /* 18 */
        "   Brightness Down  ",                                       /* 19 */
        " Color Saturation Up",                                       /* 20 */
        "ColorSaturation Down",                                       /* 21 */
        "      Bass Up       ",                                       /* 22 */
        "      Bass Down     ",                                       /* 23 */
        "      Treble Up     ",                                       /* 24 */
        "     Treble Down    ",                                       /* 25 */
        "    Balance Right   ",                                       /* 26 */
        "    BalanceLeft     ",                                       /* 27 */
        "   TV Contrast Up   ",                                       /* 28 */
        "  TV Contrast Down  ",                                       /* 29 */
        "   TV Search Up     ",                                       /* 30 */
        "  TV tint-hue Down  ",                                       /* 31 */
        "   TV CH Prog Up    ",                                       /* 32 */
        "   TV CH ProgDown   ",                                       /* 33 */
        "TV Last prog-channel",                                       /* 34 */
        " TV Select language ",                                       /* 35 */
        " TV Spacial Stereo  ",                                       /* 36 */
        "  TV Stereo Mono    ",                                       /* 37 */
        "  TV Sleep Timer    ",                                       /* 38 */
        "   TV tint-hue Up   ",                                       /* 39 */
        "   TV RF Switch     ",                                       /* 40 */
        "   TV Store-VOTE    ",                                       /* 41 */
        "      TV Time       ",                                       /* 42 */
        "   TV Scan Fwd Inc  ",                                       /* 43 */
        "    TV Decrement    ",                                       /* 44 */
        "      Reserved      ",                                       /* 45 */
        "   TV control-menu  ",                                       /* 46 */
        "    TV Show Clock   ",                                       /* 47 */
        "      TV Pause      ",                                       /* 48 */
        "   TV Erase Entry   ",                                       /* 49 */
        "     TV Rewind      ",                                       /* 50 */
        "     TV Goto        ",                                       /* 51 */
        "     TV Wind        ",                                       /* 52 */
        "     TV Play        ",                                       /* 53 */
        "     TV Stop        ",                                       /* 54 */
        "     TV Record      ",                                       /* 55 */
        "    TV External 1   ",                                       /* 56 */
        "    TV External 2   ",                                       /* 57 */
        "     Reserved       ",                                       /* 58 */
        "     TV Advance     ",                                       /* 59 */
        "   TV TXT-TV toggle ",                                       /* 60 */
        "  TV System StandBy ",                                       /* 61 */
        "TV Picture Crispener",                                       /* 62 */
        "    System Select   ",                                       /* 63 */
        "     Reserved       ",                                       /* 64 */
        "     Reserved       ",                                       /* 65 */
        "     Reserved       ",                                       /* 66 */
        "     Reserved       ",                                       /* 67 */
        "     Reserved       ",                                       /* 68 */
        "     Reserved       ",                                       /* 69 */
        "  TV Speech Music   ",                                       /* 70 */
        "  DIM Local Display ",                                       /* 71 */
        "     Reserved       ",                                       /* 72 */
        "     Reserved       ",                                       /* 73 */
        "     Reserved       ",                                       /* 74 */
        "     Reserved       ",                                       /* 75 */
        "     Reserved       ",                                       /* 76 */
        "Inc Control Setting ",                                       /* 77 */
        "Dec Control Setting ",                                       /* 78 */
        "   TV Sound Scroll  ",                                       /* 79 */
        "      Step Up       ",                                       /* 80 */
        "     Step Down      ",                                       /* 81 */
        "      Menu On       ",                                       /* 82 */
        "      Menu Off      ",                                       /* 83 */
        "     AV Status      ",                                       /* 84 */
        "      Step Left     ",                                       /* 85 */
        "      Step Right    ",                                       /* 86 */
        "     Acknowledge    ",                                       /* 87 */
        "      PIP On Off    ",                                       /* 88 */
        "      PIP Shift     ",                                       /* 89 */
        "    PIP Main Swap   ",                                       /* 90 */
        "    Strobe On Off   ",                                       /* 91 */
        "     Multi Strobe   ",                                       /* 92 */
        "      Main Frozen   ",                                       /* 93 */
        "  3div9 Multi scan  ",                                       /* 94 */
        "      PIPSelect     ",                                       /* 95 */
        "      MultiPIP      ",                                       /* 96 */
        "     Picture DNR    ",                                       /* 97 */
        "     Main Stored    ",                                       /* 98 */
        "     PIP Strobe     ",                                       /* 99 */
        "    Stored Picture  ",                                       /* 100 */
        "      PIP Freeze    ",                                       /* 101 */
        "      PIP Step Up   ",                                       /* 102 */
        "    PIP Step Down   ",                                       /* 103 */
        "    TV PIP Size     ",                                       /* 104 */
        "  TV Picture Scroll ",                                       /* 105 */
        " TV Actuate Colored ",                                       /* 106 */
        "       TV Red       ",                                       /* 107 */
        "       TV Green     ",                                       /* 108 */
        "      TV Yellow     ",                                       /* 109 */
        "      TV Cyan       ",                                       /* 110 */
        "    TV Index White  ",                                       /* 111 */
        "      TV Next       ",                                       /* 112 */
        "     TV Previous    ",                                       /* 113 */
        "      Reserved      ",                                       /* 114 */
        "      Reserved      ",                                       /* 115 */
        "      Reserved      ",                                       /* 116 */
        "      Reserved      ",                                       /* 117 */
        "      Sub Mode      ",                                       /* 118 */
        "   Option Sub Mode  ",                                       /* 119 */
        "      Reserved      ",                                       /* 120 */
        "      Reserved      ",                                       /* 121 */
        "TV Store Open Close ",                                       /* 122 */
        "      Connect       ",                                       /* 123 */
        "     Disconnect     ",                                       /* 124 */
        "      Reserved      ",                                       /* 125 */
        "  TV Movie Expand   ",                                       /* 126 */
        "  TV Parental Access"                                        /* 127 */
   };

/**
  * @}
  */

/** @defgroup RC5_DECODE_Private_Variables
  * @{
  */

/* Logic table for rising edge: every line has values corresponding to previous bit.
   In columns are actual bit values for given bit time. */
const tRC5_lastBitType RC5_logicTableRisingEdge[2][2] =
{
  {RC5_ZER ,RC5_INV}, /* lastbit = ZERO */
  {RC5_NAN ,RC5_ZER}, /* lastbit = ONE  */
};

/* Logic table for falling edge: every line has values corresponding to previous bit. 
   In columns are actual bit values for given bit time. */
const tRC5_lastBitType RC5_logicTableFallingEdge[2][2] =
{
  {RC5_NAN ,RC5_ONE},  /* lastbit = ZERO */
  {RC5_ONE ,RC5_INV},  /* lastbit = ONE  */
};


__IO StatusYesOrNo RC5FrameReceived = NO; /*!< RC5 Frame state */ 
__IO tRC5_packet   RC5TmpPacket;          /*!< First empty packet */

/* RC5  bits time definitions */
uint16_t  RC5MinT = 0;
uint16_t  RC5MaxT = 0;
uint16_t  RC5Min2T = 0;
uint16_t  RC5Max2T = 0;
static uint32_t TIMCLKValueKHz = 0; /*!< Timer clock */
uint16_t RC5TimeOut = 0;
uint32_t RC5_Data = 0;
RC5_Frame_TypeDef RC5_FRAME;
extern __IO uint8_t RFDemoStatus;
extern __IO uint8_t CECDemoStatus;
/**
  * @}
  */

/** @defgroup RC5_DECODE_Private_FunctionPrototypes
  * @{
  */
static uint8_t RC5_GetPulseLength (uint16_t pulseLength);
static void RC5_modifyLastBit(tRC5_lastBitType bit);
static void RC5_WriteBit(uint8_t bitVal);
static uint32_t TIM_GetCounterCLKValue(void);

/**
  * @}
  */

/** @defgroup RC5_Private_Functions
  * @{
  */
  
/**
  * @brief  RCR receiver demo exec.
  * @param  None
  * @retval None
  */
void Menu_RC5Decode_Func(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  /* Disable the TIM15, LED Toggling  */
  TIM_Cmd(TIM15, DISABLE);
  
  /* Disable the JoyStick interrupt */
  Demo_IntExtOnOffCmd(DISABLE);
  
  while (Menu_ReadKey() != NOKEY)
  {}
  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);   
  
  /* Display Image */
  LCD_SetDisplayWindow(120, 192, 64, 64);
  Storage_OpenReadFile(120, 192, "STFILES/TV.BMP");  
  LCD_WindowModeDisable();
  
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_9, "  To exit press UP  ");
  
  /* Initialize the InfraRed application: RC5 */
  RFDemoStatus = RC5DEMO; 
  RC5_Init();
  
  while(Menu_ReadKey() != UP)
  {   
    /* Decode the RC5 frame */
    RC5_Decode(&RC5_FRAME);
  }
  
  LCD_Clear(LCD_COLOR_WHITE);
  
  TIM_DeInit(TIM2);
  /* Time Base configuration 100ms*/
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1000;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0x0C80;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  /* Channel 1, 2, 3 and 4 Configuration in Timing mode */
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0x0;  
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
 
  Demo_LedShow(ENABLE);
  
  /* Exit the RF5 demo */
  RFDemoStatus = 0;
  
  /* Enable the JoyStick interrupt */
  Demo_IntExtOnOffCmd(ENABLE);
  
  /* Display menu */
  Menu_DisplayMenu();
}

/**
  * @brief  De-initializes the peripherals (RCC,GPIO, TIM)       
  * @param  None
  * @retval None
  */
void RC5_DeInit(void)
{ 
  TIM_DeInit(IR_TIM);
  GPIO_DeInit(IR_GPIO_PORT);
}

/**
  * @brief  Initialize the RC5 decoder module ( Time range)
  * @param  None
  * @retval None
  */
void RC5_Init(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  
  /*  Clock Configuration for TIMER */
  RCC_APB1PeriphClockCmd(IR_TIM_CLK , ENABLE);

  /* Enable Button GPIO clock */
  RCC_AHBPeriphClockCmd(IR_GPIO_PORT_CLK , ENABLE);
 
  /* Pin configuration: input floating */
  GPIO_InitStructure.GPIO_Pin = IR_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(IR_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig( IR_GPIO_PORT,IR_GPIO_SOURCE,GPIO_AF_2);
  
  /* Enable the TIM global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = IR_TIM_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
    
  /* TIMER frequency input */
  TIM_PrescalerConfig(IR_TIM, TIM_PRESCALER, TIM_PSCReloadMode_Immediate);
  
  TIM_ICStructInit(&TIM_ICInitStructure);
  
  /* TIM configuration */
  TIM_ICInitStructure.TIM_Channel = IR_TIM_Channel;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  
  TIM_PWMIConfig(IR_TIM, &TIM_ICInitStructure); 

  /* Timer Clock */
  TIMCLKValueKHz = TIM_GetCounterCLKValue()/1000; 

  /* Select the TIM Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(IR_TIM, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(IR_TIM, TIM_SlaveMode_Reset);

  /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(IR_TIM, TIM_MasterSlaveMode_Enable);

  /* Configures the TIM Update Request Interrupt source: counter overflow */
  TIM_UpdateRequestConfig(IR_TIM,  TIM_UpdateSource_Regular);
   
  RC5TimeOut = TIMCLKValueKHz * RC5_TIME_OUT_US/1000;

  /* Set the TIM auto-reload register for each IR protocol */
  IR_TIM->ARR = RC5TimeOut;
  
  /* Clear update flag */
  TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);

  /* Enable TIM Update Event Interrupt Request */
  TIM_ITConfig(IR_TIM, TIM_IT_Update, ENABLE);
    
  /* Enable the CC2/CC1 Interrupt Request */
  TIM_ITConfig(IR_TIM, TIM_IT_CC2, ENABLE);
    /* Enable the CC2/CC1 Interrupt Request */
  TIM_ITConfig(IR_TIM, TIM_IT_CC1, ENABLE);

  /* Enable the timer */
  TIM_Cmd(IR_TIM, ENABLE);  
  
  if (CECDemoStatus == 0)
  {
    /* Set the LCD Back Color */
    LCD_SetBackColor(LCD_COLOR_RED);
    
    /* Set the LCD Text Color */
    LCD_SetTextColor(LCD_COLOR_GREEN);    
    LCD_DisplayStringLine(LCD_LINE_0, "   STM320518-EVAL   ");
    LCD_DisplayStringLine(LCD_LINE_1, " RC5 InfraRed Demo  ");
    LCD_SetBackColor(LCD_COLOR_BLUE);
    
    /* Set the LCD Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);  
  }
  
  /* Bit time range */
  RC5MinT = (RC5_T_US - RC5_T_TOLERANCE_US) * TIMCLKValueKHz / 1000;
  RC5MaxT = (RC5_T_US + RC5_T_TOLERANCE_US) * TIMCLKValueKHz / 1000;
  RC5Min2T = (2 * RC5_T_US - RC5_T_TOLERANCE_US) * TIMCLKValueKHz / 1000;
  RC5Max2T = (2 * RC5_T_US + RC5_T_TOLERANCE_US) * TIMCLKValueKHz / 1000;
  
  /* Default state */
  RC5_ResetPacket();
}

/**
  * @brief  Decode the IR frame (ADDRESS, COMMAND) when all the frame is 
  *         received, the IRFrameReceived will equal to YES.
  *         The IRFrameReceived is set to YES inside the  IR_DataSampling() 
  *         function when the whole IR frame is received without any error.
  *         If any received bit timing is out of the defined range, the IR packet
  *         is reset and the IRFrameReceived is set to NO.
  *         After the IR received display, the IRFrameReceived is set NO.
  *         User can check the IRFrameReceived variable status to verify if there
  *         is a new IR frame already received.          
  * @param  rc5_frame: pointer to IR_Frame_TypeDef structure that contains the 
  *         the IR protocol fields (Address, Command,...).
  * @retval None
  */
void RC5_Decode(RC5_Frame_TypeDef *rc5_frame)
{ 
  /* If frame received */
  if(RC5FrameReceived != NO)  
  {       

    RC5_Data = RC5TmpPacket.data ;
    
    /* RC5 frame field decoding */
    rc5_frame->Address = (RC5TmpPacket.data >> 6) & 0x1F;
    rc5_frame->Command = (RC5TmpPacket.data) & 0x3F; 
    rc5_frame->FieldBit = (RC5TmpPacket.data >> 12) & 0x1;
    rc5_frame->ToggleBit = (RC5TmpPacket.data >> 11) & 0x1;
    
    /* Check if command ranges between 64 to 127:Upper Field */
    if (rc5_frame->FieldBit == 0x00)
    {
      rc5_frame->Command =  (1<<6)| rc5_frame->Command; 
    }
    
    if (CECDemoStatus == 0)
    {
     
      /* Display RC5 message */
      LCD_DisplayStringLine(LCD_LINE_6, rc5_Commands[rc5_frame->Command]);
      LCD_DisplayStringLine(LCD_LINE_7, rc5_devices[rc5_frame->Address]);

    }  
      /* Default state */
    RC5FrameReceived = NO;
    
    RC5_ResetPacket();
    
  }  
}

/**
  * @brief  Set the incoming packet structure to default state.
  * @param  None
  * @retval None
  */
void RC5_ResetPacket(void)
{
  RC5TmpPacket.data = 0;
  RC5TmpPacket.bitCount = RC5_PACKET_BIT_COUNT - 1;
  RC5TmpPacket.lastBit = RC5_ONE;
  RC5TmpPacket.status = RC5_PACKET_STATUS_EMPTY;
}

/**
  * @brief  Identify the RC5 data bits.
  * @param  rawPulseLength: low/high pulse duration
  * @param  edge: '1' for Rising  or '0' for falling edge
  * @retval None
  */
void RC5_DataSampling(uint16_t rawPulseLength, uint8_t edge)
{ 
  uint8_t pulse;
  tRC5_lastBitType tmpLastBit;
  
  /* Decode the pulse length in protocol units */
  pulse = RC5_GetPulseLength(rawPulseLength);

  /* On Rising Edge */
  if (edge == 1)
  { 
    if (pulse <= RC5_2T_TIME) 
    { 
      /* Bit determination by the rising edge */
      tmpLastBit = RC5_logicTableRisingEdge[RC5TmpPacket.lastBit][pulse];
      RC5_modifyLastBit (tmpLastBit);
    }
    else
    {
      RC5_ResetPacket();
    }
  } 
  else     /* On Falling Edge */
  {
    /* If this is the first falling edge - don't compute anything */
    if (RC5TmpPacket.status & RC5_PACKET_STATUS_EMPTY)
    { 
      RC5TmpPacket.status &= (uint8_t)~RC5_PACKET_STATUS_EMPTY;
    }
    else	
    {
      if (pulse <= RC5_2T_TIME) 
      { 
        /* Bit determination by the falling edge */
        tmpLastBit = RC5_logicTableFallingEdge[RC5TmpPacket.lastBit][pulse];
        RC5_modifyLastBit (tmpLastBit);
      }
      else
      {
        RC5_ResetPacket();
      }
    }
  }
}  

/**
  * @brief  Convert raw pulse length expressed in timer ticks to protocol bit times.
  * @param  pulseLength:pulse duration
  * @retval bit time value
  */
static uint8_t RC5_GetPulseLength (uint16_t pulseLength)
{ 
  /* Valid bit time */
  if ((pulseLength > RC5MinT) && (pulseLength < RC5MaxT))
  { 
    /* We've found the length */
    return (RC5_1T_TIME);	/* Return the correct value */
  }
  else if ((pulseLength > RC5Min2T) && (pulseLength < RC5Max2T))
  {
    /* We've found the length */
    return (RC5_2T_TIME);/* Return the correct value */
  }
  return RC5_WRONG_TIME;/* Error */
}

/**
  * @brief  perform checks if the last bit was not incorrect. 
  * @param  bit: where bit can be  RC5_NAN or RC5_INV or RC5_ZER or RC5_ONE
  * @retval None
  */
static void RC5_modifyLastBit(tRC5_lastBitType bit)
{
  if (bit != RC5_NAN)
  {
    if (RC5TmpPacket.lastBit != RC5_INV)
    { 
      /* Restore the last bit */
      RC5TmpPacket.lastBit = bit;

      /* Insert one bit into the RC5 Packet */
      RC5_WriteBit(RC5TmpPacket.lastBit);
    }
    else 
    {
      RC5_ResetPacket();
    }
  }
}

/**
  * @brief  Insert one bit into the final data word. 
  * @param  bitVal: bit value 'RC5_ONE' or 'RC5_ZER'
  * @retval None
  */
static void RC5_WriteBit(uint8_t bitVal)
{
  /* First convert RC5 symbols to ones and zeros */
  if (bitVal == RC5_ONE)
  { 
    bitVal = 1;
  }
  else if (bitVal == RC5_ZER)
  {
    bitVal = 0;
  }
  else
  {
    RC5_ResetPacket();
    return;
  } 

  /* Write this particular bit to data field */
  RC5TmpPacket.data |=  bitVal; 
  
  /* Test the bit number determined */ 
  if (RC5TmpPacket.bitCount != 0)  /* If this is not the last bit */
  {
    /* Shift the data field */
    RC5TmpPacket.data = RC5TmpPacket.data << 1;
    /* And decrement the bitCount */
    RC5TmpPacket.bitCount--;
  } 
  else
  {
    RC5FrameReceived = YES;
  }
}

/**
  * @brief  Identify TIM clock
  * @param  None
  * @retval Timer clock
  */
static uint32_t TIM_GetCounterCLKValue(void)
{
  uint32_t apbprescaler = 0, apbfrequency = 0;
  uint32_t timprescaler = 0;
  __IO RCC_ClocksTypeDef RCC_ClockFreq;   
  
  /* This function fills the RCC_ClockFreq structure with the current
  frequencies of different on chip clocks */
  RCC_GetClocksFreq((RCC_ClocksTypeDef*)&RCC_ClockFreq);
  
  /* Get the clock prescaler of APB1 */
  apbprescaler = ((RCC->CFGR >> 8) & 0x7);
  apbfrequency = RCC_ClockFreq.PCLK_Frequency; 
  timprescaler = TIM_PRESCALER;
  
  /* If APBx clock div >= 4 */
  if (apbprescaler >= 4)
  {
    return ((apbfrequency * 2)/(timprescaler + 1));
  }
  else
  {
    return (apbfrequency/(timprescaler+ 1));
  }
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
