bool time_check(int interval);
bool time_in_check(int interval);
void time_setPrev();
void time_trigger(unsigned long prev, int interval);

/**
 * @brief Maps value to remaining time to interval from start
 * 
 * @param interval
 * @param valFrom 
 * @param valTo 
 * @param from 
 * @param to 
 * @return int 
 */
int time_map(int interval, int valFrom, int valTo, int from, int to);

bool time_anim();

bool time_test(int interval);
bool time_clear(int interval);

bool time_sensor();
bool time_ISR();
bool time_shake();