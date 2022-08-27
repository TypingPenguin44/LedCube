# LED Cube
### Video: https://youtu.be/NiWNA6QWti0
### Video of the animations: https://youtu.be/tz9rI_uMX1A
### Description: A cube made out of PCBs with 54 RGB LEDs a webserver and an accelerometer for pretty animations.
### Original Idea: https://www.youtube.com/watch?v=6NPTslF68Q0
The hardware is 80% the same, the pcbs I designed myself after mostly copying the schematics but still managed to break some things which were "fixed" without being able to get a second version of the PCB.

Software is 100% **my** work.

This is full rewrite of my code from 2021 when I started this project coding wise.

It needed a refactoring becasue it was written in a way that is dumb, unscaleable and unreadable.

### In the future
- A way to solve a "Rubik's cube" on the LedCube
- Dice and other sensor driven animations flicker if at the tipping point between sides
- Add a Type-C port and usb-serial converter for easier programming and charging

## Small bit about the hardware
The cube is made of six 30x30mm PCBs but with the leds and the PCBs being atttached by the edges final dimensions are: 37x37x37mm.

Stuff inside:
- ESP01f microcontroller
- ADXL345 accelerometer
- Latch circuit for the single button 
- Battery charging ic
- 330mah Li-Po battery

### Charging the cube
Its complicated... I never had the patience to test and wait out the some hour charging of the cube with the charging ic from a normal 5v phone charger (with the usb cable cut up...) so I have been charging it from a lab bench PSU directly.

Ideally in the next iteration I will put a Type-C plug on it.

## Software
I used the Arduino platform not esp-idf because I was familiar with it.

All code needed to be non-blocking because the MCU is running a webserver + WiFi and with delay() or other easier timing conventions that wouldn't be possible.

The code is made of 15 files: 
- 12 c/c++ and header files
- 1 html page
- 2 configuration files

# Usage

With a simple button press the cube turns on. After the little startup animations the default Rainbow animation starts.

With a double press (press button twice in 250ms) it switches to MODE 1.
To go back simply initiate a double press again.

With a long press (button held for 1s) it enters MODE 2, sleep mode.
To go back just hold the button for 1s.

With an even longer press (button held for 3s) it switches to MODE 3, charging.
To go back just hold the button for 1s.

## There are 10 + 2 animations in total:

- **Rainbow:** All leds sweep through HSB/HSL hue continuously
- **Snake:** 2 leds go around the cube on a pre determined route like a snake
- **Snake++:** Same as simple snake with a twist, when the snake reaches the end of a side it becomes 1 segment longer, if it reaches max length it starts shrinking at the end of the sides
- **Donut:** the center led brightness increases, when at max all 8 other leds light up then start decreasing in birghtness until fully dark (its the same on all 6 sides)
- **Diagonal:** a 3 led "snake" goes diagonally around all sides, at the corners it randomly goes left or right
- **D-pad:** 4 corners increase in brightness until full, then a dpad shape lights up slowly decreasing in brightness leavign the center lit until it fully turns off (its the same on all 6 sides)

With the help of the ADXL345 accelerometer I could make animations which react to user input in the form of tilting / rotating.

- **Dice:** It lights up the only side up representing a dice. If you actually roll the cube a "rolling" animation would start and play until the cube is rolling. After it would reveal the number. If the cube is shaken it would show a scramble animation revealing the number at the end. 
- **Bubble:** It lights up the corner which is the highest up, like a bubble would rise to the top of a liquid
- **Loading:** It shows a loading animation on whichever side is on top, it switches direction with a shake
- **Lines:** With the cube sitting on a table it would show a line "spinning" around an axis. With tilting it would move 1 line to the left or right. With a shake it would switch axis where with tilting it would once again move 1 line to the left or right. With a 3rd shake it would spin around the last axis. With a 4th shake it would start from the beginning.

The button is **not** the only form of switching animations or modes.

Shake to cycle can be turned on which works on all animations that do not have special functionality on shake.

Another way is the complete "web" interface.  
The ESP01f is wifi capable so with soft AP mode it can host its own wifi network an webserver which you can connect to with your phone and control everything on the cube. 

```
SSID: LedCube
PW: OooShiny
IP: 192.168.4.1
```
(The password can only be changed in the code but as this device is not intended to be connected to the internet / no one has one except me I did not implement a way to change the AP password)


# About the code in more detail
(If unfamiliar with platformio the projects files are in /data, /src, and the modified library in /lib)

### The modified NeoPixelBus lib:
I had errors compiling with the current settings in platformio.ini with the selected (at the begginging definietly newest) version of the library. After some modifications the code compiled fine.

### defs.h:
Globally used variables get declared here.

### index.html: 
I was going for a somewhat dark mode look. I couldn't use Bootstrap or the like becasue the cube does not have internet accesss so the page was coded by hand. (Thanks W3schools <3)

### main.cpp:
The core of the code.

### gfx.cpp:
Code in this file makes the LEDs shine as this file is responsible for all the animations.

### io.cpp: 
This code handles inputs from the button and form the accelerometer and changes modes and states based on input.

- MODE 0: Normal animation mode nothing too fancy
- MODE 1: Animations which react to adx345 sensor readings
- MODE 2: System on, No lights, practically sleep mdoe
- MODE 3: Simple charging indicator

### network.cpp:
This code initializes wifi and the webserver. Requests from the web interface are handled here.

### settings.cpp:
SPIFFS gets initialized and read for saved configurations. Saving loading and resetting of the config files is possible with these functions.

### timers.cpp: 
With these functions animations and sensor checks get called at the right time with the correct "delay" in between. These functions ensure that the animations won't speed up on higher clockspeed microcontrollers while still being able to run on a single core single thread system without hiccups or dropouts in the wifi connection.

### All other header files:
They define functions so other parts of code can use them not having to worry about the order in which they were #included.


