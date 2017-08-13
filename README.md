# What?
An electronic conference badge, of course! (That makes this a wearable right?).

If you're interested in keeping up to date with new versions, add your email here: https://docs.google.com/forms/d/e/1FAIpQLSfi4PA6UNeILMCECjqM97Cd1tH6X2Ml2tzJlvIlYt14cOBuXQ/viewform

## Yes, but what does it do?
By default the device does pleasant RGB fades across its 10 LEDs based on an internal clock. It also beacons that clock (and some other metadata) in infrared. They use any beacons they notice to synchronize their clocks, which also synchronizes the RGB patterns. The effect is that if one is alone, it plays random colors. If a group of them are together, they tend towards traversing the colorspace together.

This is a complete rewrite of the work done hastily in https://github.com/borgel/sympetrum for [DEFCON](https://defcon.org/) 24. The goal is to deploy at DEFCON 25 this year.

## Why is it a dragonfly?
If you haven't read Neal Stephenson's [_The Diamond Age: or, A Young Lady's Illustrated Primer_](http://www.goodreads.com/book/show/827.The_Diamond_Age), do that first. I'll wait.

If you have, there's a sequence around page 300 in which Miranda is first introduced to the possibility of using the drummers to find Nell. In this scene she attends a party in which the revellers wear colorful cloisonné dragonfly pins. As the party progresses they transition from random colorful flickering to some sort of synchrony, and Miranda notices the participants do the same. This is what I built.

A [_Sympetrum_](https://en.wikipedia.org/wiki/Sympetrum) is a genus of Dragonfly.

# How Do I Make One?
Gerbers and electrical sources in the [Hardware](Hardware/) directory! Firmware for this version currently in progress, but the hardware has been validated. Ping me for more.

