#include <Arduino.h>
unsigned long time_now;
unsigned long time_previous_press;


bool time_check(unsigned long interval) {
  time_now = millis();
  
  if (interval == 0){
    time_previous_press = time_now;
	return 1;
  }
 
  if ((time_now - time_previous_press) >= interval) {
	  time_previous_press = time_now; 
    return 1;
  }
  
  return 0;
}
void time_setPrev() {
  time_previous_press = millis();
}


int time_map(int interval, int valFrom, int valTo, int from, int to)
{
  unsigned long now = millis();
  return map((time_previous_press + interval) - now, valFrom, valTo, from, to);
  
}