#ifndef IO_H
#define IO_H

#include <defs.h>
#include <Arduino.h>

void io_startReset();
void io_check();
void io_handler();
void io_sensorRead();
void io_accelSetup();
void io_batteryCheck(bool override);
void io_getCharge();

#endif
