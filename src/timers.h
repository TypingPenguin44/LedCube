#ifndef TIMERS_H
#define TIMERS_H

/**
 * @brief If x amount of time has passed returns true
 * 
 * @param interval amount of time to check
 * @return true 
 * @return false 
 */
bool time_check(unsigned int interval);
/**
 * @brief If x amount of time HASNT passed yet returns true
 * 
 * @param interval 
 * @return true 
 * @return false 
 */
bool time_in_check(unsigned int interval);
/**
 * @brief sets previous time of event to time_now
 */
void time_setPrev();
/**
 * @brief Maps value to remaining time to interval, from start
 * 
 * @param interval
 * @param valFrom 
 * @param valTo 
 * @param from 
 * @param to 
 * @return int 
 */
int time_map(unsigned int interval, int valFrom, int valTo, int from, int to);

bool time_anim();

bool time_test(unsigned int interval); //test prints time
bool time_clear(unsigned int interval); //clear time in MODE 2

bool time_sensor();
bool time_ISR();
bool time_shake();
bool time_chargeCheck();
bool time_scramble(); 
bool time_freeze();

#endif