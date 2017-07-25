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

bool led_Init(void);
bool led_StartAnimation(void);

// YABI is used internally for all LED control
bool led_SetChannel(uint32_t id, struct color_ColorHSV c);
bool led_SetChannelTimed(uint32_t id, struct color_ColorHSV c, uint32_t timeMS);

void led_SetBiasValue(uint8_t biasValue);
void led_SetBiasWeight(uint8_t biasWeight);
void led_SetAnimationSpeeds(uint32_t frameTime, uint32_t transitionTime);

void led_GiveTime(uint32_t systimeMS);

#endif//LED_H__

