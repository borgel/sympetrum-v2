# Make One
If you're interested in making a Sympetrum badge, you're going to need two things: blank PCBs to assemble and the parts to assemble them with. Everything you'll need can be easily found on this project's [Kitnic page](https://kitnic.it/boards/github.com/borgel/sympetrum-v2/). 

## PCBs
Hit the button on the top of the Kitnic page that says "Download Gerbers". You'll get a zip file of the electrical files which describe the circuit board in a way that can be consumed by a fabrication house. There are a wide range of options for tuning these into circuit board you can use. Fab's I've purchased boards from and been happy include [PCBWay](https://www.pcbway.com), [Seeed Studio](https://www.seeedstudio.com/fusion_pcb.html) and [Dirty PCBs](https://dirtypcbs.com/store/pcbs). There are many other options. These boards are designed to fit in the common 100mm x 100mm size which is often heavily discounted. Expect these to cost around $25 (with shipping!) for 10 in any color you want (they were designed for white solder mask / black silkscreen, but the design should work in any color!).

### A Note on OSHPark
I love OSHPark and use them for all sorts of projects! Unfortunately for boards this large they are quite a bit more expensive than the Chinese options (around $60 for three boards).

## Parts
The easiest way to order parts is again to use Kitnic. Below the images of the board there is a quantity selector and a Digikey button. If you press the button the webpage will magically open a Digikey card with all the parts in it (nifty isn't it?). At the time of writing it costs around $13 to build a single board from Digikey

If you'd rather do things the hard way you can directly upload the BOM to [Digikey](digikey.com) or a tool like [Octopart](https://octopart.com/bom-tool) which will make it easy to order from a variety of suppliers.

### Caveats
* APA102C's, (AKA [DotStars on Adafruit](https://www.adafruit.com/product/2343)) may not be in stock on Digikey. If not, you can [buy them from Adafruit](https://www.adafruit.com/product/2343). You will need 10 for one dragonfly, though I recommend getting extra to fix ones that get damaged during assembly. They are also _much cheaper_ from EBay or Aliexpress so consider those options if you're comfortable or more cost sensitive.
* The battery holder may not exactly fit the through holes on the PCB. They were custom sized to fit exactly the cheap holders I got on [Aliexpress](aliexpress.com). You could try lining one leg up and forcing the other to bend. Or use a holder with wires and gluing/taping it into place.

### Alternative Parts
Nothing in this design is particularly sensitive. If some specific part I selected is out of stock it should be safe to replace with a random part with the same parameters.

#### CPU
Sometimes the CPU seems to disappear from the common US suppliers. If this happens and you don't want to source from elsewhere the [STM32F051K8T6](https://www.digikey.com/product-detail/en/stmicroelectronics/STM32F051K8T6/497-13626-ND/3947378) can be swapped in place of the normal CPU. It's slightly fancier and will require you to recompile the firmware for it, but no other changes are required.

To do this edit [the Makefile](/Firmware/Makefile) and swap the `CPU` variable by commenting out the line for `STM32F030x6` and in the one for `STM32F051x8`.


# Raw Sources
The final release gerbers are named `FF1.1` (formfactor 1.1) are [here](FF1.1). This is exactly what the Kitnic link mentioned above links to. This the final design which was used at DEFCON25.

The firmware is ["version 4"](https://github.com/borgel/sympetrum-v2/releases/tag/v4) and is tagged as a release on GitHub (and in [the Firmware directory](/Firmware)).


# Editiing/Contributing
If you want to make modifications, all design files made with [KiCAD](http://kicad-pcb.org/), a "Cross Platform and Open Source Electronics Design Automation Suite." It's very powerful, but has a steep learning curve.

