# LED Cube
### Video: 
### Description:
 ctrl k and v
 Original Idea: https://www.youtube.com/watch?v=6NPTslF68Q0
 (hardware 80% the same the pcbs i designed myself after mostly copying the schematics)

 Software is 100% **my** work

 This is full refactor of my code from 2021 when i started this project coding wise.

 It needed a refactoring becasue it was written in a way that is ***unpublishable***.

 ## Small bit about the hardware

 ## Software
Arduino platform not esp-idf because I was familiar with it.

All code needs to be non-blocking because the cube is running a webserver + wifi and with delay() or other easier timing conventions that wouldn't be possible.



MODE 0: Normal animation mode nothing too fancy
MODE 1: Animations which react to adx345 sensor
MODE 2: System on, No lights
MODE 3: Simple charging indicator

