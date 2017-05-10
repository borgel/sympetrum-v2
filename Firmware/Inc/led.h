#ifndef LED_H__
#define LED_H__

/*
 * A thin driver for the APA102's used together in a group.
 */

#include <stdint.h>
#include <stdbool.h>

#include "color.h"
#include "stm32f0xx_hal.h"

#define MAX_LED_CHAIN_LENGTH       10

bool led_Init(void);
bool led_SetChannel(uint8_t ch, struct color_ColorRGB color);
bool led_UpdateChannels(void);

#endif//LED_H__

