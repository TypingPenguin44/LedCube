#ifndef DEFS_H
#define DEFS_H

#include <Arduino.h>
#include <Adafruit_ADXL345_U.h>
/*
* Global variables which are used everywhere
*/

#define NUM_LEDS 54

//cube pinout 
#define latch 14//5 // 14 // pin # of latch that keeps the device on
#define BTN 12//13 // 12 // pin # of button
//#define LED_PIN 13//4 //13 // useless because lib uses fixd pin 2


//testing on 10x10 matrix pinout
/*#define latch 5 // 14 // pin # of latch that keeps the device on
#define BTN 13 // 12 // pin # of button
#define LED_PIN 4 //13*/

//timings in ms
#define DOUBLE_PRESS_TIME 250
#define LONG_PRESS_TIME 1000
#define SLEEP_PRESS_TIME 3000
#define DEBOUNCE 60
#define SENSOR_TIME 20

//other constant values
#define gfx_normal 6
#define gfx_accel 5
#define NUM_ANIM 13
#define NUM_TOGGLES 2

//struct to store each animation delay last trigger time and sensor requirement
typedef struct {
  unsigned long last_trig;
  unsigned int interval;
  bool adxl;
} animations;

//create arrays to store the data
extern animations gfx[NUM_ANIM];
extern bool toggles[2];

extern unsigned long time_now;

extern int MODE;
extern bool rendered;
extern bool clear;
extern bool static_colors;

extern int BATTERY;

extern int current_anim;
extern float gfx_h;
extern float gfx_s;
extern float gfx_v;
extern Adafruit_ADXL345_Unified accel;
extern int gfx_dice_side;

//input states
extern bool single_press;
extern bool double_press;
extern bool long_press;
extern bool sleep_press;
extern bool shake;

extern unsigned long time_ISR_prev;

extern bool SHUTDOWN;

extern bool startfade;

//helper values for animations
extern int gfx_lines_roll[3];
extern int gfx_lines_axis;

extern int gfx_bubble_corner;
extern bool gfx_dice_scramble;

extern int gfx_scramble_length;
extern int gfx_bubble_fromto[2];
extern bool gfx_bubble_transition;
extern bool gfx_loading_clkwise;

#endif