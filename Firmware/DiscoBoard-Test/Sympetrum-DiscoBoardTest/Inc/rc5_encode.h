/**
  ******************************************************************************
  * @file    rc5_encode.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the rc5 encode 
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
#ifndef __RC5_ENCODE_H
#define __RC5_ENCODE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
   
/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup RC5_ENCODE
  * @{
  */


/** @defgroup RC5_ENCODE_Exported_Types
  * @{
  */
   
/**
  * @}
  */

/** @addtogroup RC5_ENCODE_Exported_Types
  * @{
  */

   /**
  * @brief Definition of the RC5 Control bit value.
  */        
typedef enum
{
  RC5_Ctrl_Reset                        = ((uint16_t)0),
  RC5_Ctrl_Set                          = ((uint16_t)0x0800)
}RC5_Ctrl_TypeDef;

/**
  * @}
  */

/** @defgroup RC5_ENCODE_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup RC5_ENCODE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup RC5_Exported_Functions
  * @{
  */

void Menu_RC5_Encode_Func(void);
void RC5_Encode_Init(void);
void RC5_Encode_SendFrame(uint8_t RC5_Address, uint8_t RC5_Instruction, RC5_Ctrl_TypeDef RC5_Ctrl);
void RC5_Encode_SignalGenerate(void);

#ifdef __cplusplus
}
#endif

#endif  /*__RC5_ENCODE_H */
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
