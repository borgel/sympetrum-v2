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

// Get the period of the hue clock for a given beacon interval
#define HUE_PERIOD_MS_FOR_BEACON(x)       ((x) / 255)

// Parallel arrays used to set clock intervals
static const uint16_t BeaconIntervalRampMS[] = {5000, 2500, 1250, 625};

// STATE STUFF
// Fast hue clock. The period is = the time between ticks of the Beacon Clock.
// That means it needs to tick 255 times during the Beacon interval
static uint8_t HueClock;
static uint16_t HueClockPeriod;
static uint32_t LastHueClockTime;

// Slow beacon clock
static uint16_t BeaconClock;
static uint16_t BeaconClockInterval;
static uint16_t BeaconClockRampPosition;
static uint32_t LastBeaconClockTime;

static void pattern_Permute(void);

void pattern_Init(void) {
   BeaconClockRampPosition = 0;
   BeaconClock = 0;
   BeaconClockInterval = BeaconIntervalRampMS[BeaconClockRampPosition];
   // Start one tick in to allow for time manipulation
   LastBeaconClockTime = BeaconClockInterval;

   HueClock = 0;
   HueClockPeriod = HUE_PERIOD_MS_FOR_BEACON(BeaconClockInterval);
   LastHueClockTime = HueClockPeriod;

   //TODO pass in a CB for each RX'd beacon?
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

   // XXX On Beacon tick (infrequent)
   if(systimeMS - LastBeaconClockTime > BeaconClockInterval) {
      LastBeaconClockTime = systimeMS;

      //FIXME don't send hue, that doesn't matter
      iprintf("Beaconing (Hue %d)\n", HueClock);

      beacon_Send(HueClock);

      // Reset Hue clock too
      HueClock = 0;
      LastHueClockTime = systimeMS;

      // TODO regress beacon interval ramp to slow it down
   }

   //XXX On Hue tick (frequent)
   if(systimeMS - LastHueClockTime > HueClockPeriod) {
      LastHueClockTime = systimeMS;

      iprintf("H", HueClock);

      //TODO set animation bias to HueClock instead of raw hue set

      {
         // Max S and Max V are brightest colors
         struct color_ColorHSV c = {.h = HueClock, .s = 255, .v = 255};

         // now set LEDs to that color
         for(int i = 0; i < 10; i++) {
            led_SetChannel(i, c);
         }
      }

      // this is the 'actual' tick
      HueClock++;
   }
}

static void pattern_Permute(void) {
   //TODO
}

void pattern_SawBeacon(uint16_t rawBeacon) {
   //TODO react to incoming beacon in complex way
   // FIXME how much should we jump the clock forward? X% of interval?
   // Moving last time back is == moving next time sooner
   LastHueClockTime -= 50;

   //TODO advance beacon interval ramp to speed it up?
}
