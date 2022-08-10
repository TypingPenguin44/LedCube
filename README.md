# LED Cube
### Video: 
### Description:
ctrl k and v
Original Idea: https://www.youtube.com/watch?v=6NPTslF68Q0
Hardware: 80% the same, the pcbs I designed myself after mostly copying the schematics but still managed to break some things which were "fixed" without being able to get a second version of the pcb

Software is 100% **my** work

This is full rewrite of my code from 2021 when i started this project coding wise.

It needed a refactoring becasue it was written in a way that is ***unpublishable***.

## Small bit about the hardware
The cube is made of 30x30mm pcbs but with the leds and the pcbs being atttached by the edges the final dimensions are 36x36x36mm.
Stuff inside:
ESP01f
ADXL345
Latch circuit for the single button 
Battery charging chip
330mah Li-Po battery


## Software
I used the Arduino platform not esp-idf because I was familiar with it.

All code needed to be non-blocking because the cube is running a webserver + wifi and with delay() or other easier timing conventions that wouldn't be possible.

MODE 0: Normal animation mode nothing too fancy
MODE 1: Animations which react to adx345 sensor readings
MODE 2: System on, No lights, practically sleep mdoe
MODE 3: Simple charging indicator

After pressign the button the cube turns on. After the little startup animations the default is MODE 0 Animation 0 which is the rainbow animation.

With a double press (press button twice in 250ms) it switches to MODE 1.
To go back simply initiate a double press again.

With a long press (button held for 1s) it enters MODE 2, sleep mode.
To go back just hold the button for 1s.

With an even longer press (button held for 3s) it switches to MODE 3, charging.
To go back just hold the button for 1s.

There are 9 animations in total: 
rainbow: all leds sweep through HSB/HSL hue continuously
snake: 2 leds go around the cube on a pre determined route like a snake
snake+: same as simple snake with a twist, when the snake reaches the end of a side it becomes 1 segment longer, if it reaches max length it starts shrinking at the end of the sides
donut: the center led brightness increases, when at max all 8 other leds light up then start decreasing in birghtness until fully dark (its the same on all 6 sides)
diagonal: a 3 led "snake" goes diagonally around all sides, at the corners it randomly goes left or right
dpad: 4 corners increase in brightness until full, then a dpad shape lights up slowly decreasing in brightness leavign the center lit until it fully turns off (its the same on all 6 sides)

With the help of the ADXL345 accelerometer I could make animations which react to user input in the form of tilting.

bubble: It lights up the corner which is the highest up, like a bubble would rise to the top of a liquid
dice: It lights up the only side up representing a dice. If you actually roll the cube a "rolling" animation would start and play until the cube is rolling. After it would reveal the number. If the cube is shaken it would show a scramble animation revealing the number at the end. 
lines: With the cube sitting on a table it would show a line "spinning" around axis #. With tilting it would move 1 line to the left or right. With a shake it would switch axis to # where with tilting it would once again move 1 line to the left or right. With a 3rd shake it would spin around the Z axis. With a 4th shake it would start from the beginning.

The button is not the only form of switching animations or modes.
Shake to cycle can be turned on but only for MODE 0 animations, which do not react to the sensor.

Another way is the complete "web" interface. 
The ESP01f is wifi capable so with soft AP mode it can host its own wifi network which you can connect to with your phone and control everything on the cube.



