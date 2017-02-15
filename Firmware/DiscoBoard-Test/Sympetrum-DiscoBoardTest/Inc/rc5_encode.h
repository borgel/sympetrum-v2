#ifndef __RC5_ENCODE_H
#define __RC5_ENCODE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx.h"

typedef enum
{
  RC5_Ctrl_Reset                        = ((uint16_t)0),
  RC5_Ctrl_Set                          = ((uint16_t)0x0800)
} RC5_Ctrl_TypeDef;

void RC5_Encode_Init(void);
void RC5_Encode_SendFrame(uint8_t RC5_Address, uint8_t RC5_Instruction, RC5_Ctrl_TypeDef RC5_Ctrl);
void RC5_Encode_SignalGenerate(void);

#ifdef __cplusplus
}
#endif

#endif  /*__RC5_ENCODE_H */

