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

enum BeaconIntervalChoice {
   BIC_Increase,
   BIC_Decrease,
};

// Parallel arrays used to set clock intervals
#define BEACON_INTERVAL_RAMP_LEN          (6)
static const uint16_t BeaconIntervalRampMS[BEACON_INTERVAL_RAMP_LEN] =
   {15000, 10000, 5000, 2500, 1250, 625};
//FIXME better way to calculate this automatically
static const uint16_t BiasWeightRamp[BEACON_INTERVAL_RAMP_LEN] =
   {0    , 10   , 20  , 30  , 50  , 70};

// Amount to bump Beacon clock time when a beacon is seen
// 10% of total value?
#define GET_BEACON_BUMP(x)                ((x) / 10)

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

static void pattern_SetBeaconInterval(enum BeaconIntervalChoice c);
static void pattern_UpdateAnimation(uint8_t hue);
static void pattern_UpdateSimpleHue(uint8_t hue);

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

   led_StartAnimation();
   //led_SetAnimationSpeeds(BeaconClockInterval, BeaconClockInterval);
   //led_SetAnimationSpeeds(BeaconClockInterval, HueClockPeriod);
   //led_SetAnimationSpeeds(HueClockPeriod, HueClockPeriod);

   //weirdly this is closest to the correct behavior? we get slow (gentle) color
   //animation because of the slow transition time, but frequent updates because
   //of the fast animation framerate
   led_SetAnimationSpeeds(HueClockPeriod * 5, BeaconClockInterval);
}

void pattern_GiveTime(uint32_t const systimeMS) {
   uint16_t lastBeacon;

   if(beacon_Receive(&lastBeacon)) {
      // If we saw a beacon, handle it
      pattern_SawBeacon(lastBeacon);
   }

   //  On Beacon tick (infrequent)
   if(systimeMS - LastBeaconClockTime > BeaconClockInterval) {
      LastBeaconClockTime = systimeMS;

      //FIXME don't send hue, that changes and doesn't matter
      //CRC8 of ID?
      iprintf("Beaconing (Hue %d)\n", HueClock);

      beacon_Send(HueClock);

      // Reset Hue clock too
      HueClock = 0;
      LastHueClockTime = systimeMS;

      // TODO regress beacon interval ramp to slow it down
      pattern_SetBeaconInterval(BIC_Decrease);
   }

   // On Hue tick (frequent)
   if(systimeMS - LastHueClockTime > HueClockPeriod) {
      LastHueClockTime = systimeMS;

      iprintf("H", HueClock);

      //pattern_UpdateSimpleHue(HueClock);
      pattern_UpdateAnimation(HueClock);

      // this is the 'actual' tick
      HueClock++;
   }
}

/*
 * When running the animation, update the channel bias and value.
 */
static void pattern_UpdateAnimation(uint8_t hue) {
   //TODO get weight from table
   led_SetBiasValue(hue);

   //TODO set speeds
   //led_SetAnimationSpeeds(BeaconClockInterval, HueClockPeriod);
   //led_SetAnimationSpeeds(BeaconClockInterval, BeaconClockInterval);
}

/*
 * When not running the animation, update LED hues directly.
 */
static void pattern_UpdateSimpleHue(uint8_t hue) {
   // Max S and Max V are brightest colors
   struct color_ColorHSV c = {.h = HueClock, .s = 255, .v = 255};

   // now set LEDs to that color
   for(int i = 0; i < 10; i++) {
      led_SetChannel(i, c);
   }
}

void pattern_SawBeacon(uint16_t rawBeacon) {
   uint16_t beaconBump;

   //TODO advance beacon interval ramp to speed it up?
   pattern_SetBeaconInterval(BIC_Increase);

   // Now that we have the new Beacon period, continue

   // Jump the beacon clock 'forward' when a beacon comes in
   // Moving last time back is == moving next time sooner
   //FIXME rm
   iprintf("LastClock %d --(", LastHueClockTime);
   beaconBump = GET_BEACON_BUMP(BeaconClockInterval);
   LastHueClockTime -= beaconBump;
   iprintf("%d)--> %d\n", beaconBump, LastHueClockTime);

   //FIXME do we want this? Causes a jump (but there would be one either way)
   // Rescale the Hue clock too so we still get a full cycle, but faster
   HueClockPeriod = HUE_PERIOD_MS_FOR_BEACON(BeaconClockInterval - beaconBump);

   //TODO update animation speed too
   led_SetAnimationSpeeds(HueClockPeriod * 5, BeaconClockInterval);

   //TODO update bias
   led_SetBiasWeight(BiasWeightRamp[BeaconClockRampPosition]);
}

static void pattern_SetBeaconInterval(enum BeaconIntervalChoice c) {
   //FIXME rm
   iprintf("BeaconRamp %d ->", BeaconClockRampPosition);

   if(c == BIC_Increase) {
      // Move UP interval ramp

      BeaconClockRampPosition++;
      if(BeaconClockRampPosition > BEACON_INTERVAL_RAMP_LEN) {
         BeaconClockRampPosition = BEACON_INTERVAL_RAMP_LEN;
      }
   }
   else if(c == BIC_Decrease) {
      // Move DOWN interval ramp

      if(BeaconClockRampPosition > 0) {
         BeaconClockRampPosition--;
      }
   }

   //FIXME rm
   iprintf(" %d", BeaconClockRampPosition);

   BeaconClockInterval = BeaconIntervalRampMS[BeaconClockRampPosition];
   HueClockPeriod = HUE_PERIOD_MS_FOR_BEACON(BeaconClockInterval);
}

