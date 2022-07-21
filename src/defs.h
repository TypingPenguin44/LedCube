#ifndef DEFS_H
#define DEFS_H
/*
* Global variables which are used everywhere
*/
#include <Arduino.h>

#define NUM_LEDS 54
#define latch 5 // 14 // pin # of latch that keeps the device on
#define BTN 13 // 12 // pin # of button
#define LED_PIN 4 //13
#define DOUBLE_PRESS_TIME 250
#define LONG_PRESS_TIME 1000
#define SLEEP_PRESS_TIME 3000
#define DEBOUNCE 60

typedef struct {
  unsigned long last_trig;
  int interval;
  int place;
} animations;

extern animations gfx[10];

extern uint8_t MODE;
extern bool rendered;
extern bool clear;
extern unsigned long time_now;

extern int current_anim;

#endif