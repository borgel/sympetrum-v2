#ifndef LED_H__
#define LED_H__

/*
 * A thin driver for the APA102's used together in a group.
 */

#include <stdint.h>
#include <stdbool.h>

#include "color.h"
#include "stm32f0xx_hal.h"
#include "yabi/yabi.h"

#define LED_CHAIN_LENGTH       10

bool led_Init(void);

// YABI is used for all LED control
bool led_SetChannel(uint32_t id, struct color_ColorHSV c);
bool led_SetSubChannel(uint32_t id, yabi_ChanValue val, uint32_t timeMS);

//TODO set clock fxn

void led_GiveTime(uint32_t systimeMS);

#endif//LED_H__

