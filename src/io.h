#ifndef IO_H
#define IO_H

#include <defs.h>
#include <Arduino.h>

/**
 * @brief If button held 3s on startup resets all json settings and restarts the cube
 */
void io_startReset();
/**
* @brief Check button for input
* @def Could rewrite it with interrupts in mind, Wasnt possible with previous library
*/
void io_check();
void io_handler();
void io_sensorRead();
void io_accelSetup();
/**
 * @brief Gets called periodically, if battery low it turns off the cube
 * @param override Overide disables turn off on low battery
 */
void io_batteryCheck(bool override = false);
void io_getCharge();


#endif
