/*
Basic Behavior:
when 3/part, dragonflies do a random slow fade across all LEDS
when two or more people are together, their badges obviously synchronize colors but are still somewhat random and slow
when 3/4+ people are together, their badges animations speed up and are nearly the same color

See https://workflowy.com/s/E7Zu.sFDTbsFAk9

References:
The way real fireflies do it: ncase.me/fireflies/
*/

#include "pattern.h"
#include "beacons.h"

#include "iprintf.h"
#include <stdint.h>

#define        BEACON_INTERVAL_MS      ( 5000 )

void pattern_Init(void) {
   //TODO pass in a CB for each RX'd beacon
   beacon_Init(BEACON_INTERVAL_MS);
}

void pattern_GiveTime(uint32_t systimeMS) {
   beacon_GiveTime(systimeMS);

   //TODO react to beacon
}
