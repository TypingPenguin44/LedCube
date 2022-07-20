#ifndef BUTTON_H
#define BUTTON_H

#include <defs.h>
#include <Arduino.h>
#include <time.h>
#include <gfx.h>


#ifdef DEBUG_ESP_PORT
  #ifndef DEBUG_MSG
  #define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__"\n")
  #endif
#else
#define DEBUG_MSG(...)
#endif

bool single_press = false;
bool double_press = false;
bool long_press = false;



int buf = 0;

void startupReset();
void inputCheck();


#endif

  

/**
* @brief Check button for input
* @def Have to do this the hard way without any more interrupts because Fastled with ws2812 uses interrupts
* and it causes problems with wifi on a single threaded cpu like the esp8285 
*/
void inputCheck() {

  if(MODE == 0 || MODE == 1 || MODE == 2 || MODE == 3)
  {
    if(digitalRead(BTN) == HIGH) // if button is pressed
    {
      time_setPrev(); // set previous btn press to now

      if (single_press == false) // if not btn press before set it true
      {
        single_press = true;
      }
      // if pressed AND pressed again in 250ms register as double press
      else if(single_press == true && time_check(DOUBLE_PRESS_TIME))
      {
        while(digitalRead(BTN) == HIGH){} //if still pressed infinite loop
        double_press = true;
        single_press = false;
      }
    }

    while (digitalRead(BTN) == HIGH) {
      buf = time_map(1000, 900, 0, 0, 8);
      clear = true;
      if (buf > 1) // if buf more than one call anim
      {
        gfx_loading(buf, 0);
      } 
      delay(5);
      if (digitalRead(BTN) == HIGH && time_check(LONG_PRESS_TIME)) {  //if button held down more than 1s

        if (MODE == 0 || MODE == 1) {
          clear = true;
          MODE = 2;
        } else if(MODE == 2) {
          MODE = 0;
        }
        while (digitalRead(BTN) == HIGH) { //if button still pressed
          buf = time_map(3000, 3000, 0, 0, 8);
          gfx_loading(buf, 1);
          delay(5);
          if (digitalRead(BTN) == HIGH && time_check(SLEEP_PRESS_TIME)) { //after 3s of holding exec
            if (MODE == 0 || MODE == 1 || MODE == 2) { //remove mode == 2 to only able to go into charge from mode2 and holding to superlong
              MODE = 3;
              clear = false;
            } else if(MODE == 3) {
              MODE = 0;
            }
            while(digitalRead(BTN) == HIGH) {} //infinite loop while still holding
          }
        }
        single_press = false; //set to false to prevent anim increment while trying to switch modes
      }
    }
  }
  if(MODE == 4){
    if(digitalRead(BTN) == HIGH){
      time_setPrev();
      if (single_press == false){
        single_press = true;
      }else if(single_press == true && time_check(DOUBLE_PRESS_TIME)){
        while(digitalRead(BTN) == HIGH){}
        double_press = true;
        single_press = false;
      }
    }

    while(digitalRead(BTN) == HIGH){
      buf = time_map(1500, 1500, 0, 0, 8);
      if (buf > 1){
        gfx_loading(buf, 0);
      } // if buf more than one call anim
      delay(5);
      rendered = false;
      if(digitalRead(BTN) == HIGH && time_check(1500)){  //if button held down more than 1500ms 
        MODE = 2;
        clear = true;
        while(digitalRead(BTN) == HIGH){} //if button still pressed
        single_press = false; //set to false to prevent anim increment while trying to switch modes
      }
    }
  }
  if(MODE == 5){
    if(digitalRead(BTN) == HIGH){
      time_setPrev();
      single_press = true;
      while(digitalRead(BTN) == HIGH){}
    }
  }
  /*

  //handle buttonpresses
  if(singlePress == true && currMillis - lastButtonPress >= 250){ //250ms after a press it executes    
    s = 255; //set hsv values to max in case it was modified in the webview 
    v = 255;
    singlePress = false;
    staticMode = false;
    debugEnd = currMillis;
    //staticMode = false;
    if(MODE == 0){ //if in normal mode increment normal anim
      animNormal++;
      if(animNormal >= normalWrapAround){ //wrap around
        animNormal = 0;
      }
      univDelay = delays[animNormal];
    }else if (MODE == 1){ //if in double mode increment double anim
      animDouble++;
      if(animDouble >= 2){ //wrap around
        animDouble = 0;
      }
      univDelay = doubleDelays[animDouble];
    }else if (MODE == 4){
      menuPointer++;
      rendered = false;
      while(menuContent[menuPointer][0] == 14){
        menuPointer++;
        if(menuPointer >= 9){
          menuPointer = 0;
        }
      }
      if(menuPointer >= 9){
          menuPointer = 0;
      }
    }else if(MODE == 5){
      MODE = 4;
    }
  }else if (doublePress == true){
    debugEnd = currMillis;
    if(MODE == 0){
      MODE = 1;
      s = 255;
      v = 255;
      //staticMode = false;
      univDelay = doubleDelays[animDouble];
    }
    else if(MODE == 1){
      MODE = 0;
      s = 255;
      v = 255;
      //staticMode = false;
      univDelay = delays[animNormal];
    }else if(MODE == 2){
      MODE = 4;
      rendered = false;
    }else if(MODE == 4){
      menuSelect();
      rendered = false;
    }
    singlePress = false;
    doublePress = false;
  }else if(shake == true){
    if(animDouble == 1){
      loadingReverse = !loadingReverse;
      loadingPointer = 0;
    }
    shake = false;
  }*/
}