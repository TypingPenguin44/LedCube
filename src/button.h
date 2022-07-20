#ifndef BUTTON_H
#define BUTTON_H

#include <defs.h>
#include <Arduino.h>


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
bool clear = false;

int buf = 0;

void startupReset();
void inputCheck();
int map_to(int value, int valFrom, int valTo, int from, int to);

Wait wt = Wait();

extern uint8_t MODE;

#endif

  

/**
* @brief Check button for input
* @def Have to do this the hard way without any more interrupts because Fastled with ws2812 uses interrupts
* and it causes problems with wifi on a single threaded cpu like the esp8285 
*/
void inputCheck() {

  /*if(MODE == 0 || MODE == 1 || MODE == 2 || MODE == 3)
  {
    if(digitalRead(BTN) == HIGH) // if button is pressed
    {
      wt.set_prev(); // set previous btn press to now
      wt.set(DOUBLE_PRESS_TIME);

      if (single_press == false) // if not btn press before set it true
      {
        single_press = true;
      }
      // if pressed AND pressed again in 250ms register as double press
      else if(single_press == true && wt.check())
      {
        while(digitalRead(BTN) == HIGH){} //if still pressed infinite loop
        double_press = true;
        single_press = false;
      }
    }

    while(digitalRead(BTN) == HIGH){
      buf = wt.map_to(1000, 900, 0, 0, 8);
      clear = true;
      if (buf > 1) // if buf more than one call anim
      {
        loading(buf, 0);
      } 
      delay(5);
      if(digitalRead(BTN) == HIGH && currMillis - lastButtonPress >= 1000){  //if button held down more than 1s
        lastButtonPress = currMillis;
        if(MODE == 0 || MODE == 1){
          clear = true;
          MODE = 2;
        }else if(MODE == 2){
          MODE = 0;
        }
        while(digitalRead(BTN) == HIGH){ //if button still pressed
          buf = map((lastButtonPress+3000)-currMillis, 3000, 0, 0, 8); //call animation
          longpress(buf, 1);
          delay(5);
          if(digitalRead(12) == HIGH && currMillis - lastButtonPress >= 3000){ //after 3s of holding exec
            if(MODE == 0 || MODE == 1 || MODE == 2){ //remove mode == 2 to only able to go into charge from mode2 and holding to superlong
              MODE = 3;
              clear = false;
            }else if(MODE == 3){
              MODE = 0;
            }
            while(digitalRead(12) == HIGH){} //infinite loop while still holding
          }
        }
        singlePress = false; //set to false to prevent anim increment while trying to switch modes
      }
    }
  }
  if(MODE == 4){
    if(digitalRead(12) == HIGH){
      currMillis = millis();
      lastButtonPress = currMillis;
      if (singlePress == false){
        singlePress = true;
      }else if(singlePress == true && currMillis - lastButtonPress <= 250){
        while(digitalRead(BTN) == HIGH){}
        doublePress = true;
        singlePress = false;
      }
    }

    while(digitalRead(12) == HIGH){
      currMillis = millis();
      buf = map((lastButtonPress+1500)-currMillis, 1500, 0, 0, 8);
      if (buf > 1){longpress(buf, 0);} // if buf more than one call anim
      delay(5);
      rendered = false;
      if(digitalRead(12) == HIGH && currMillis - lastButtonPress >= 1500){  //if button held down more than 1500ms 
        lastButtonPress = currMillis;
        MODE = 2;
        clear = true;
        while(digitalRead(12) == HIGH){} //if button still pressed
        singlePress = false; //set to false to prevent anim increment while trying to switch modes
      }
    }
  }
  if(MODE == 5){
    if(digitalRead(12) == HIGH){
      currMillis = millis();
      lastButtonPress = currMillis;
      singlePress = true;
      while(digitalRead(BTN) == HIGH){}
    }
  }
  currMillis = millis();
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

int map_to(int value, int valFrom, int valTo, int from, int to) {
  return 0;
}