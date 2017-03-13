#ifndef COLOR_H__
#define COLOR_H__

/*
 * Generic colors and utilities. Derived from
 * https://github.com/borgel/sympetrum
 */

#include <stdint.h>

struct color_ColorRGB {
   uint8_t     b;
   uint8_t     g;
   uint8_t     r;
};

#define HSV_CHANNEL_MAX         255

struct color_ColorHSV {
   uint8_t     h;
   uint8_t     s;
   uint8_t     v;
};

//AKA white
static const struct color_ColorHSV COLOR_HSV_MAXSV = {
   .h = 0,
   .s = HSV_CHANNEL_MAX,
   .v = HSV_CHANNEL_MAX
};

void color_HSV2RGB(struct color_ColorHSV const *hsv, struct color_ColorRGB *rgb);

#endif//COLOR_H__

