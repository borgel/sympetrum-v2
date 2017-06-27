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
#include "color.h"
#include "led.h"

#include "iprintf.h"
#include <stdint.h>

static const uint16_t ClockIntervalRampMS[] = {5000, 2500, 1250, 625};

// amount to bump the clock forward when a beacon is seen
static const uint8_t ClockBump = 255/10;

// STATE STUFF
// Fast hue clock
static uint8_t HueClock;
// Slow beacon clock
static uint16_t BeaconClock;

static uint16_t ClockIntervalMS;
static uint32_t LastClockTimeMS;

static void pattern_Permute(void);

void pattern_Init(void) {
   BeaconClock = 0;
   HueClock = 0;
   ClockIntervalMS = ClockIntervalRampMS[0] / 100;
   LastClockTimeMS = 0;

   //TODO pass in a CB for each RX'd beacon
   beacon_Init();

   //FIXME en
   //led_StartAnimation();
}

void pattern_GiveTime(uint32_t const systimeMS) {
   uint16_t lastBeacon;

   if(beacon_Receive(&lastBeacon)) {
      // If we saw a beacon, handle it
      pattern_SawBeacon(lastBeacon);
   }

   // Adjust the pattern based on any beacons, etc
   pattern_Permute();

   // On Beacon
   if(systimeMS - LastClockTimeMS > ClockIntervalMS) {
      // Max S and Max V are brightest colors
      struct color_ColorHSV c = {.h = HueClock, .s = 255, .v = 255};

      LastClockTimeMS = systimeMS;

      iprintf("Beaconing (Hue %d)\n", HueClock);

      beacon_Send(HueClock);

      // now set LEDs to that color
      //TODO modify animation bias instead of raw hue set
      for(int i = 0; i < 10; i++) {
         led_SetChannel(i, c);
      }

      // uhh, how do we do this?
      HueClock++;
   }
}

static void pattern_Permute(void) {
   //TODO
   //uhh, is there actually anything to do here?
}

void pattern_SawBeacon(uint16_t rawBeacon) {
   // Dumbest choice - bump the clock
   HueClock += ClockBump;

   //TODO react to incoming beacon in complex way
}
