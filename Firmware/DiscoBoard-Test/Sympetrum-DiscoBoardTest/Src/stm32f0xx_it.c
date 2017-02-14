/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx_hal_tim_ex.h"

#include "rc5_encode.h"
#include "rc5_decode.h"

#include "iprintf.h"


/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

//FIXME move these out of this file (into RC5?)
static uint32_t ICValue1, ICValue2;

/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/
/**
 * @brief  This function handles TIM16 interrupt request.
 * @param  None
 * @retval None
 */
void TIM16_IRQHandler(void)
{
   //figure out the next stage of the outgoing signal
   RC5_Encode_SignalGenerate();

   /* Clear TIM16 update interrupt */
   __HAL_TIM_CLEAR_FLAG(&htim16, TIM_FLAG_UPDATE);
}
/**
 * @brief  This function handles TIM14 overflow and update interrupt request.
 * @param  None
 * @retval None
 */
void TIM2_IRQHandler(void)
{
   //iprintf(">");
   /* Clear the TIM2 Update pending bit */
   //TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
   __HAL_TIM_CLEAR_IT(&htim2, TIM_FLAG_UPDATE);

   /* - Timer Falling Edge Event:
    *     The Timer interrupt is used to measure the period between two 
    *     successive falling edges (The whole pulse duration).
    *
    * - Timer Rising Edge Event:  
    *     It is also used to measure the duration between falling and rising 
    *     edges (The low pulse duration).
    *     The two durations are useful to determine the bit value. Each bit is 
    *     determined according to the last bit. 
    *
    *  Update event:InfraRed decoders time out event.
    *  ---------------------------------------------
    *     It resets the InfraRed decoders packet.
    *     - The Timer Overflow is set to 3.6 ms .*/
   /* IC1 Interrupt*/
   //if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC2) != RESET))
   if(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC2) != RESET)
   {
      //TIM_ClearFlag(IR_TIM, TIM_FLAG_CC2);
      __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC2);

      /* Get the Input Capture value */
      //ICValue2 = TIM_GetCapture2(IR_TIM);
      //param is channel 1-4
      //FIXME not sure... what's on CH2?
      //ICValue2 = HAL_TIM_ReadCapturedValue(&htim2, 2);
      ICValue2 = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
      //ICValue2 = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);

      //FIXME rm
      //iprintf("<O2 %d]", ICValue2);

      /* RC5 */
      RC5_DataSampling(ICValue2 - ICValue1 , 0);
   }  /* IC2 Interrupt */
   //else if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC1) != RESET))
   else if(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC1) != RESET)
   {
      //TIM_ClearFlag(IR_TIM, TIM_FLAG_CC1);
      __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC1);

      /* Get the Input Capture value */
      //ICValue1 = TIM_GetCapture1(IR_TIM);
      //param is channel 1-4
      //ICValue1 = HAL_TIM_ReadCapturedValue(&htim2, 1);
      ICValue1 = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);

      //FIXME rm
      //iprintf("<O1 %d]", ICValue1);

      RC5_DataSampling(ICValue1 , 1);
   } 
   /* Checks whether the IR_TIM flag is set or not.*/
   //else if ((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_Update) != RESET))
   else if(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET)
   {
      //FIXME rm
      iprintf("R\r\n\r\n]");

      /* Clears the IR_TIM's pending flags*/
      //TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);
      __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);

      RC5_ResetPacket(); 
   }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
