## Ehx9-All: combining and programming EHX '9' pedals

The '9' series of pedals is a range of polyphonic synthesis pedals for guitar. The
effects and sounds that can be obtained are superb, however they have a few shortcomings: 
first of all they are not programmable (i.e. there's only one setting, no way to store 
favourites and recall them quickly) and if you have more than one, they tend to take
up quite a lot of space on a pedalboard, especially if one uses them only sparingly
during a set.

The modifications documented in this repository try to eliminate these shortcomings.

## Programmability

To store and recall settings, the pots and switch are removed. GPIOs from the
microcontroller are connected to the main board to replace the functionality of
the pots and the switch. Since we use a PWM signal for the pots, it is low-pass
filtered by a simple first-order RC network.

Encoders take the place of the pots and the switch, a TFT is added for visual 
feedback of the settings.

## Space saving

All pedals of this serie share the same PCB. The difference is in the firmware of the
DSP. This firmware is stored in an EEPROM. By multiplexing multiple EEPROMS on one
board and resetting the DSP after switching the multiplexer, one can switch between
pedals of the series within one enclosure.

## Implementing the mods

Modding a pedal is not for the faint of heart. It is pretty easy to destroy the pedal
in the process. Also, a number of tools are needed:

* A way to program an Attiny2313. I use a Tuxgraphics AVR programmer, but there's a large number of other options.
* A way to program the STM32 blackpill. I use an ST-Link clone.
* There's SMD (de) soldering involved
* The enclosure is modified

## BOM

The BOMs for the PCBs can be found in their folders:
[controller](./controller-pcb/ibom.html)
[multiplexer](./multiplexer-pcb/ibom.html)

## Firmware

The multiplexer firmware was made with the Arduino IDE.

The controller firmware was made with VSCode + PlatformIO.

