#ifndef PATTERN_H__
#define PATTERN_H__

#include <stdint.h>

//FIXME move config out into struct?
void pattern_Init(void);
void pattern_GiveTime(uint32_t const systimeMS);

void pattern_SawBeacon(uint16_t rawBeacon);

#endif//PATTERN_H__

