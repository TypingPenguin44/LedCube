

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "betterDelay.h"

Wait::Wait()
{
	
	this->interval_millis = 1000;
	
}


Wait::Wait(unsigned long interval_millis)
{
	
	this->interval_millis = interval_millis;
	
}

void Wait::set(unsigned long interval_millis)
{
  this->interval_millis = interval_millis;
}

uint8_t Wait::check()
{

  unsigned long now = millis();
  
  if (interval_millis == 0){
    previous_millis = now;
	return 1;
  }
 
  if ((now - previous_millis) >= interval_millis) {
	  previous_millis = now; 
    return 1;
  }
  
  return 0;

}

void Wait::set_prev()
{
  unsigned long now = millis();
  previous_millis = now;
}

void Wait::set_prev(unsigned long previous)
{
  previous_millis = previous;
}

int Wait::map_to(int value, int valFrom, int valTo, int from, int to)
{
  unsigned long now = millis();
  return map((previous_millis + value) - now, valFrom, valTo, from, to);
  
}

void Wait::reset() 
{
 
  this->previous_millis = millis();

}


