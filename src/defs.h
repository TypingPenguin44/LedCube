/*
* Global variables which are used everywhere
*/
#include <Arduino.h>

#define NUM_LEDS 54
#define latch 14 // pin # of latch that keeps the device on
#define BTN 12 // pin # of button
#define DOUBLE_PRESS_TIME 250
#define LONG_PRESS_TIME 1000
#define SLEEP_PRESS_TIME 3000


extern uint8_t MODE;
extern bool rendered;
extern bool clear;