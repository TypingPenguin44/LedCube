#ifndef DEFS_H
#define DEFS_H
/*
* Global variables which are used everywhere
*/
#include <Arduino.h>
#include <Adafruit_ADXL345_U.h>

#define NUM_LEDS 54
#define latch 5 // 14 // pin # of latch that keeps the device on
#define BTN 13 // 12 // pin # of button
#define LED_PIN 4 //13
#define DOUBLE_PRESS_TIME 250
#define LONG_PRESS_TIME 1000
#define SLEEP_PRESS_TIME 3000
#define DEBOUNCE 60
#define gfx_normal 4
#define gfx_gyro 0
#define SENSOR_TIME 20


typedef struct {
  unsigned long last_trig;
  int interval;
  bool adxl;
} animations;

extern animations gfx[10];

extern uint8_t MODE;
extern bool rendered;
extern bool clear;
extern unsigned long time_now;
extern int BATTERY;

extern int current_anim;
extern uint8_t gfx_h;
extern uint8_t gfx_s;
extern uint8_t gfx_v;
extern Adafruit_ADXL345_Unified accel;
extern uint8_t gfx_dice_side;

extern bool shakeCycle;

#endif