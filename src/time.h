bool time_check(unsigned long interval);
void time_setPrev();

/**
 * @brief Maps value to remaining time to interval end
 * 
 * @param interval Time condition true
 * @param valFrom 
 * @param valTo 
 * @param from 
 * @param to 
 * @return int 
 */
int time_map(int interval, int valFrom, int valTo, int from, int to);