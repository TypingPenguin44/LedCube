#include <defs.h>
#include <Arduino.h>
#include <time.h>
#include <gfx.h>
#include <button.h>
#include <Adafruit_ADXL345_U.h>
#include <settings.h>


#ifdef DEBUG_ESP_PORT
  #ifndef DEBUG_MSG
  #define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__)
  #endif
#else
#define DEBUG_MSG(...)
#endif


bool single_press = false;
bool double_press = false;
bool long_press = false;
bool sleep_press = false;

int buf = 0;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/**
* @brief Check button for input
* @def Have to do this the hard way without any more interrupts because Fastled with ws2812 uses interrupts
* and it causes problems with wifi on a single threaded cpu like the esp8285 
*/
void button_check() {
  if(digitalRead(BTN) == HIGH) // if button is pressed
  {
    if (single_press == false) // if not btn press before set it true
    {
      single_press = true;
    } // if pressed AND pressed again in 250ms register as double press
    else if(single_press == true && time_in_check(DOUBLE_PRESS_TIME))
    {
      while(digitalRead(BTN) == HIGH){} //if still pressed infinite loop
      double_press = true;
      single_press = false;
      DEBUG_MSG("[button_check] Double press\n");
    }
    time_setPrev(); // set previous btn press to now
  }

  while (digitalRead(BTN) == HIGH) {
    buf = time_map(1000, 900, 0, 0, 8);
    if (buf > 1) // if buf more than one call anim
    {
      gfx_loading(buf, 0);
    } 
    delay(5);
    if (time_check(LONG_PRESS_TIME)){  //if button held down more than 1s
      long_press = true;
      single_press = false;
      double_press = false;
      
      while (digitalRead(BTN) == HIGH) { //if button still pressed
        buf = time_map(3000, 3000, 0, 0, 8);
        gfx_loading(buf, 1);
        delay(5);
        
        if (time_check(SLEEP_PRESS_TIME)){ //after 3s of holding exec
          sleep_press = true;
          long_press = false;
          single_press = false;
          double_press = false;
          
          while(digitalRead(BTN) == HIGH) {yield();} //infinite loop while still holding
        }
      }
    }
  }
  button_handler();
}

void button_handler() {
  if(single_press == true && time_check(DOUBLE_PRESS_TIME)){ //250ms after a press it executes    
    single_press = false;
    DEBUG_MSG("[button_check] Single press\n");
    gfx_reset();
    
    if(MODE == 0){ //if in normal mode increment normal anim
      current_anim++;
      if(current_anim >= gfx_normal){ //wrap around
        current_anim = 0;
      }
    }else if (MODE == 1){ //if in double mode increment double anim
      current_anim++;
      if(current_anim >= 9){ //wrap around
        current_anim = 8; //####
      }
    }
  }else if (double_press == true){
    double_press = false;
    gfx_reset();

    if(MODE == 0){
      MODE = 1;
      gfx_clear();
      current_anim = 8;
    }
    else if(MODE == 1){
      MODE = 0;  
      gfx_clear();
      current_anim = 0;
    }    
  }/*else if(shake == true){
    if(animDouble == 1){
      loadingReverse = !loadingReverse;
      loadingPointer = 0;
    }
    shake = false;
  }*/

  if(long_press){
    long_press = false;
    DEBUG_MSG("[button_check] Long press\n");
    if (MODE == 0 || MODE == 1) {
        gfx_clear();
        MODE = 2;
      } else if(MODE == 2) {
        MODE = 0;
      }
  }
  if(sleep_press){
    sleep_press = false;
    DEBUG_MSG("[button_check] Sleep press\n");
    if (MODE == 0 || MODE == 1 || MODE == 2) { 
      gfx_clear();
      MODE = 3;
      current_anim = 9; //set anim to charge
    } else if(MODE == 3) {
      MODE = 0;
      current_anim = 0;
    }
  }
}

void button_accelSetup(){
  if(!accel.begin()){ //adxl345 setup
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    Serial.println("Disabling sensor...");
  }else{
    accel.setRange(ADXL345_RANGE_8_G); //SetRange 16, 8, 4, 2
  }
}

void button_sensorRead(){
  if(time_sensor()){
    sensors_event_t event; 
    accel.getEvent(&event);
  
    int x = event.acceleration.x;
    int y = event.acceleration.y;
    int z = event.acceleration.z;
    
    if(z < -7){
      gfx_dice_side = 1;
    }else if(z > 7){
      gfx_dice_side = 6;
    }else if(x < -7){
      gfx_dice_side = 3;
    }else if(x > 7){
      gfx_dice_side = 4;
    }else if(y < -7){
      gfx_dice_side = 2;
    }else if(y > 7){
      gfx_dice_side = 5;
    }
  }
}

void button_startReset(){
  while (digitalRead(BTN) == HIGH) {
    buf = time_map(3000, 3000, 0, 0, 8);
    gfx_loading(buf, 1);
    delay(5);
    if(time_check(SLEEP_PRESS_TIME)){
      DEBUG_MSG("RESET...\n");
      settings_reset_delays();
      //put other settings here
      while (digitalRead(BTN) == HIGH){yield();}
    }
  }
}