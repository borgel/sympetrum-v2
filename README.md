# What?
An electronic conference badge, of course! (That makes this a wearable right?)

If you're interested in keeping up to date with new versions, you can add your email here: https://docs.google.com/forms/d/e/1FAIpQLSfi4PA6UNeILMCECjqM97Cd1tH6X2Ml2tzJlvIlYt14cOBuXQ/viewform. I'll start sending out info as we move towards DEFCON26.

## Yes, but what does it do?
By default the device does pleasant RGB fades across its 10 LEDs based on an internal clock. It also beacons that clock (and some other metadata) in infrared. They use any beacons they notice to synchronize their clocks, which also synchronizes the RGB patterns. The effect is that if one is alone, it plays random colors. If a group of them are together, they tend towards traversing the colorspace together.

For a little more information, it was kindly [written up by Hackaday here](https://hackaday.com/2017/07/14/badge-from-diamond-age-comes-to-def-con/).

This is a complete rewrite of the work done hastily in https://github.com/borgel/sympetrum for [DEFCON](https://defcon.org/) 24. The goal is to deploy at DEFCON 25 this year.

## Why is it a dragonfly?
If you haven't read Neal Stephenson's [_The Diamond Age: or, A Young Lady's Illustrated Primer_](http://www.goodreads.com/book/show/827.The_Diamond_Age), do that first. I'll wait.

If you have, there's a sequence around page 300 in which Miranda is first introduced to the possibility of using the drummers to find Nell. In this scene she attends a party in which the revellers wear colorful cloisonné dragonfly pins. As the party progresses they transition from random colorful flickering to some sort of synchrony, and Miranda notices the participants do the same. This is what I built.

A [_Sympetrum_](https://en.wikipedia.org/wiki/Sympetrum) is a genus of Dragonfly.

# How Do I Get One?
Unfortunately, I'm totally out. But everything is totally open source, so you can make one yourself! If you're interested check out [the Hardware Readme](Hardware/README.md) for recommendations.

The firmware used at DEFCON25 seems to work correctly and is considered "Final" barring bug discovery.

