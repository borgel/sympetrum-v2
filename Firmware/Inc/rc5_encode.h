#ifndef __RC5_ENCODE_H
#define __RC5_ENCODE_H

#include "stm32f0xx.h"
#include <stdbool.h>

typedef enum
{
   RC5_Ctrl_Reset                        = ((uint16_t)0),
   RC5_Ctrl_Set                          = ((uint16_t)0x0800)
} RC5_Ctrl_TypeDef;

void ir_InitEncode(void);
void ir_SendRC5(uint8_t RC5_Address, uint8_t RC5_Instruction, RC5_Ctrl_TypeDef RC5_Ctrl);
void ir_SendRaw(uint16_t message);
void ir_SignalGenerate(void);
bool ir_IsSending(void);

#endif  /*__RC5_ENCODE_H */

