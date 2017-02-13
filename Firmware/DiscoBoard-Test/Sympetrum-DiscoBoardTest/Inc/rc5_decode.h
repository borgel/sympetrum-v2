/**
  ******************************************************************************
  * @file    rc5_decode.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the RC5 
  *          firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RC5_DECODE_H
#define __RC5_DECODE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

#include <stdbool.h>
/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup RC5_DECODE
  * @{
  */

/** @defgroup RC5_DECODE_Exported_Types
  * @{
  */
   
/** 
  * @brief  RC5 frame structure  
  */
typedef struct
{   
  __IO uint8_t FieldBit;   /*!< Field bit */   
  __IO uint8_t ToggleBit;  /*!< Toggle bit field */
  __IO uint8_t Address;    /*!< Address field */ 
  __IO uint8_t Command;    /*!< Command field */ 
  
} RC5_Frame_TypeDef;

/** 
  * @brief  RC5 packet structure
  */
typedef struct
{
 __IO uint16_t data;     /*!< RC5 data */ 
 __IO uint8_t  status;   /*!< RC5 status */
 __IO uint8_t  lastBit;  /*!< RC5 last bit */
 __IO uint8_t  bitCount; /*!< RC5 bit count */
} tRC5_packet;

enum RC5_lastBitType
{
  RC5_ZER,
  RC5_ONE,
  RC5_NAN,
  RC5_INV
};

typedef enum RC5_lastBitType tRC5_lastBitType;

/**
  * @}
  */

/** @defgroup RC5_DECODE_Exported_Constants
  * @{
  */

#define RC5_1T_TIME                          0x00
#define RC5_2T_TIME                          0x01
#define RC5_WRONG_TIME                       0xFF
#define RC5_TIME_OUT_US                      3600
#define RC5_T_US                             900     /*!< Half bit period */
#define RC5_T_TOLERANCE_US                   270    /*!< Tolerance time */
#define RC5_NUMBER_OF_VALID_PULSE_LENGTH     2
//FIXME isn't this 14 bits?
#define RC5_PACKET_BIT_COUNT                 13      /*!< Total bits */

/* Packet struct for reception*/
#define RC5_PACKET_STATUS_EMPTY              1<<0
/**
  * @}
  */

/** @defgroup RC5_DECODE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup RC5_DECODE_Exported_Functions
  * @{
  */
void Menu_RC5Decode_Func(void);
void RC5_Decode_DeInit(void);
void RC5_Decode_Init(void);
bool RC5_Decode(RC5_Frame_TypeDef *rc5_frame);
void RC5_ResetPacket(void);
void RC5_DataSampling(uint16_t rawPulseLength, uint8_t edge);
#ifdef __cplusplus
}
#endif

#endif /* __RC5_DECODE_H */
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
