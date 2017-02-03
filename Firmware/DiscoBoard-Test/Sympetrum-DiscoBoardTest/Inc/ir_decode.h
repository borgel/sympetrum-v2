/**
  ******************************************************************************
  * @file    ir_decode.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the IR Decode 
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
#ifndef __IR_DECODE_H
#define __IR_DECODE_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include <stdio.h>
#include "rc5_decode.h"
   
/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup IR_DECODE
  * @{
  */

/** @defgroup IR_DECODE_Exported_Types
  * @{
  */
   
typedef enum { NO = 0, YES = !NO} StatusYesOrNo;
/**
  * @}
  */

/** @defgroup IR_DECODE_Exported_Constants
  * @{
  */

//#define IR_TIM                 TIM2                     /*!< Timer used for IR decoding */
//#define TIM_PRESCALER          47                       /* !< TIM prescaler */
//#define IR_TIM_CLK             RCC_APB1Periph_TIM2      /*!< Clock of the used timer */
//#define IR_TIM_IRQn            TIM2_IRQn                /*!< IR TIM IRQ */
//#define IR_TIM_Channel         TIM_Channel_2            /*!< IR TIM Channel */

//#define IR_GPIO_PORT           GPIOB                    /*!< Port which IR output is connected */
//#define IR_GPIO_PORT_CLK       RCC_AHBPeriph_GPIOB      /*!< IR pin GPIO Clock Port */
//#define IR_GPIO_PIN            GPIO_Pin_3               /*!< Pin which IR is connected */
//#define IR_GPIO_SOURCE         GPIO_PinSource3
   
//FIXME rm?
#define RC5DEMO                0x01
#define SIRCDEMO               0x02

/**
  * @}
  */

/** @defgroup IR_DECODE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup IR_DECODE_Exported_Functions
  * @{
  */


#ifdef __cplusplus
}
#endif

#endif /* __IR_DECODE_H */
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
