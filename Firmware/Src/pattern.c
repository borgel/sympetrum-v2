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

// The number of sub-sections a full 360* sweep of the color-wheel should be
// divided into. Remember these are interpolated between! They just lear the
// interpolator along.
#define HUE_CLOCK_INTERMEDIATE_SECTIONS    5
// Get the period of the hue clock for a given beacon interval
// The 2nd param (revs) is the number of full color-wheel cycles the hue clock
// should do in a single beacon clock
#define HUE_PERIOD_MS_FOR_BEACON(x, revs)       (((x) / HUE_CLOCK_INTERMEDIATE_SECTIONS) / (revs))

enum BeaconIntervalChoice {
   BIC_Increase,
   BIC_Decrease,
};

// Parallel arrays used to set clock intervals
#define BEACON_INTERVAL_RAMP_LEN          (3)
//TODO make this a 2d array for symmetry
static uint16_t const BeaconIntervalRampMS[BEACON_INTERVAL_RAMP_LEN] =
   {30000, 20000, 10000};
//FIXME better way to calculate this automatically, or tune it
static uint16_t const BiasWeightRamp[BEACON_INTERVAL_RAMP_LEN] =
   {0    , 50   , 90};

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
   BeaconClockInterval = BeaconIntervalRampMS[0];
   // Start one tick in to allow for time manipulation
   LastBeaconClockTime = BeaconClockInterval;

   HueClock = 0;
   HueClockPeriod = HUE_PERIOD_MS_FOR_BEACON(BeaconClockInterval, BeaconClockRampPosition);
   LastHueClockTime = HueClockPeriod;

   //TODO pass in a CB for each RX'd beacon?
   beacon_Init();

   led_StartAnimation();

   //weirdly this is closest to the correct behavior. we get slow (gentle) color
   //animation because of the slow transition time, but frequent updates because
   //of the fast animation framerate. Also, if the transition time ends before
   //the LED has made it, it will snap to the correct value (which is visible and
   //ugly). So Taking longer to transition is better than shorter.
   //TODO maybe try HueClockPeriod * 2 for 2nd param?
   led_SetAnimationSpeeds(HueClockPeriod, BeaconClockInterval);
   //led_SetAnimationSpeeds(HueClockPeriod, HueClockPeriod);
}

void pattern_GiveTime(uint32_t const systimeMS) {
   uint8_t trueHue;
   uint16_t lastBeacon;

   if(beacon_Receive(&lastBeacon)) {
      // If we saw a beacon, handle it
      pattern_SawBeacon(lastBeacon);
   }

   // On Hue tick (frequent)
   if(systimeMS - LastHueClockTime >= HueClockPeriod) {
      LastHueClockTime = systimeMS;

      // Figure out how many sections to break the 255 position color wheel,
      // into, then convert the current clock to that
      trueHue = (HueClock % HUE_CLOCK_INTERMEDIATE_SECTIONS)* (255 / HUE_CLOCK_INTERMEDIATE_SECTIONS);

      iprintf("H(%d) ", HueClock);

      //pattern_UpdateSimpleHue(trueHue);
      pattern_UpdateAnimation(trueHue);

      // this is the 'actual' tick
      HueClock++;
   }

   //  On Beacon tick (infrequent)
   if(systimeMS - LastBeaconClockTime > BeaconClockInterval) {
      LastBeaconClockTime = systimeMS;

      iprintf("Beacon Clock Tick!\n");

      //FIXME don't send hue, that changes and doesn't matter
      //CRC8 of ID?
      iprintf("(Hue %d) ", HueClock);

      beacon_Send(HueClock);

      // Reset Hue clock too
      HueClock = 0;
      LastHueClockTime = systimeMS;

      pattern_SetBeaconInterval(BIC_Decrease);
   }
}

/*
 * When running the animation, update the channel bias and value.
 */
static void pattern_UpdateAnimation(uint8_t hue) {
   //TODO get weight from table
   led_SetBiasValue(hue);

   //FIXME rm
   iprintf("Bias value to %d\n", hue);
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

   // Advance beacon interval ramp to speed it up
   pattern_SetBeaconInterval(BIC_Increase);

   // Now that we have the new Beacon period, continue

   // Jump the beacon clock 'forward' when a beacon comes in
   // Moving last time back is == moving next time sooner
   //TODO add jitter to the BeaconClockInterval to avoid a perfect sync
   //FIXME rm
   iprintf("LastClock %d --(", LastHueClockTime);
   beaconBump = GET_BEACON_BUMP(BeaconClockInterval);
   LastHueClockTime -= beaconBump;
   iprintf("%d)--> %d\n", beaconBump, LastHueClockTime);

   //FIXME do we want this? Causes a jump (but there would be one either way)
   // Rescale the Hue clock too so we still get a full cycle, but faster
   HueClockPeriod = HUE_PERIOD_MS_FOR_BEACON(BeaconClockInterval - beaconBump, BeaconClockRampPosition);
}

/*
 * This fires whenever the general animation behavior is changing.
 */
static void pattern_SetBeaconInterval(enum BeaconIntervalChoice c) {
   uint8_t newBias;

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

   HueClockPeriod = HUE_PERIOD_MS_FOR_BEACON(BeaconClockInterval, BeaconClockRampPosition);

   newBias = BiasWeightRamp[BeaconClockRampPosition];
   led_SetBiasWeight(newBias);
   //FIXME rm
   iprintf(" Bias weight to %d perc\n", newBias);

   led_SetAnimationSpeeds(HueClockPeriod, BeaconClockInterval);
}

