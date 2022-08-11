#ifndef BUTTON_H
#define BUTTON_H

#include <defs.h>
#include <Arduino.h>

void button_startReset();
void button_check();
void button_handler();
void button_sensorRead();
void button_accelSetup();
void button_batteryCheck(bool override);
void button_getCharge();

#endif
