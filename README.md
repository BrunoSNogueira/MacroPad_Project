# MacroPad_Project
Project of a simple Macropad, with a 4x3 (-1) matrix keyboard and a Rotary Encoder.

## Description:

Project based and inspired on 'tinyboatproductions' instructable of 'Macropad V2'.
Some of the mods:
  -- Hardware Modifications: Leds Removed;
  -- Code major modifications: Adjustments on debouncing, multikey features, allows change of music to previous, enables '*' key to also be used.
  -- Code minor mods: removed Layouts, for now (but as a choice based on my usage). And a lot of code-structure remixes. 
  
  Basicly, made it work as I wanted (and, well, actually the original code didn't work for me. Had to make some mods on the Keypad.h usage so it could print something). Now you can tap keys sequentially with a better speed. Also, media controls have a better flow.
  
  Side-notes: I do recommend running it on PlatformIO. The Arduino IDE gave me lots of problems with Keyboard prints and HID-Project.