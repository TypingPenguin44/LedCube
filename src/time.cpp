#include <Arduino.h>
#include <defs.h>

#ifdef DEBUG_ESP_PORT
  #ifndef DEBUG_MSG
  #define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__)
  #endif
#else
#define DEBUG_MSG(...)
#endif

unsigned long time_now;
unsigned long time_previous_press;



bool time_check(int interval) {
  time_now = millis();
  
  
  if (interval == 0){
    time_previous_press = time_now;
    //DEBUG_MSG("[time_check] Return True\n");
	  return 1;
  }
 
  if ((time_now - time_previous_press) >= interval) {
    /*Serial.print("Time in check time now: ");
    Serial.print(time_now);
    Serial.print(" prev: ");
    Serial.print(time_previous_press);
    Serial.print(" value: ");
    Serial.print(time_now - time_previous_press);
    Serial.print(" interval: ");
    Serial.println(interval);*/
	  time_previous_press = time_now;
    //DEBUG_MSG("[time_check] Return True\n");
    return 1;

  }
  
  return 0;
}

bool time_in_check(int interval) {
  time_now = millis();
  
  if (interval == 0){
    time_previous_press = time_now;
    //DEBUG_MSG("[time_in_check] Return True\n");
	  return 1;
  }
  //debounce might be useless in real world but int testing i had issues
  if ((time_now - time_previous_press) <= interval && (time_now - time_previous_press) >= DEBOUNCE) {
    /*Serial.print("Time in check time now: ");
    Serial.print(time_now);
    Serial.print(" prev: ");
    Serial.print(time_previous_press);
    Serial.print(" value: ");
    Serial.print(time_now - time_previous_press);
    Serial.print(" interval: ");
    Serial.println(interval);*/
	  time_previous_press = time_now;
    //DEBUG_MSG("[time_in_check] Return True\n");
    return 1;

  }
  
  return 0;
}

void time_setPrev() {
  time_now = millis();
  time_previous_press = time_now;
}


int time_map(int interval, int valFrom, int valTo, int from, int to)
{
  time_now = millis();
  return map((time_previous_press + interval) - time_now, valFrom, valTo, from, to);
  
}

bool time_anim() {
  time_now = millis();

  if ((time_now - gfx[current_anim].last_trig) >= gfx[current_anim].interval) {
    gfx[current_anim].last_trig = time_now;
    return 1;
  }

  return 0;
}
unsigned long time_testPrev = 0;

bool time_test(int interval) {
  time_now = millis();

  if (time_now - time_testPrev >= interval) {
    time_testPrev = time_now;
    return 1;
  }

  return 0;
}