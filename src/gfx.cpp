#include <defs.h>
#include <gfx.h>
#include <FastLED.h>
#include <time.h>
CRGB gfx_leds[NUM_LEDS] = {0};  // zero initialize array.

uint8_t gfx_h = 0;
uint8_t gfx_s = 255;
uint8_t gfx_v = 255;

void gfx_setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(gfx_leds, NUM_LEDS);
}

void gfx_animHandler(){
  //FastLED.clearData(); //maybe needed idk
  if (time_anim()){
    switch (current_anim){
      case 0:
        gfx_rainbow();
        break;
      case 1:
        gfx_snake();
        break;
      case 2:
        gfx_snakePlus();
        break;
      case 3:
        gfx_donut();
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        break;
      default:
        //implement yellow loading lol
        break;
    }
  }
}

void gfx_dice(uint8_t side){
  switch(side){
    case 1:
      gfx_leds[4] = CHSV(gfx_h, gfx_s, gfx_v);
    break;
    case 2:
      gfx_leds[29] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[33] = CHSV(gfx_h, gfx_s, gfx_v);
    break;
    case 3:
      gfx_leds[38] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[40] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[42] = CHSV(gfx_h, gfx_s, gfx_v);
    break;
    case 4:
      gfx_leds[18] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[20] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[24] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[26] = CHSV(gfx_h, gfx_s, gfx_v);
    break;
    case 5:
      gfx_leds[9] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[11] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[13] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[15] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[17] = CHSV(gfx_h, gfx_s, gfx_v);
    break;
    case 6:
      gfx_leds[45] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[46] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[47] = CHSV(gfx_h, gfx_s, gfx_v);
      
      gfx_leds[51] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[52] = CHSV(gfx_h, gfx_s, gfx_v);
      gfx_leds[53] = CHSV(gfx_h, gfx_s, gfx_v);
    break;
  }
  gfx_cycleColor();
  FastLED.show();
  FastLED.clearData();
}

bool gfx_donut_in = false;
void gfx_donut(){
  if(gfx_donut_in == true){
    //currentColor++;
    for(int i = 0; i < 6; i++){
      gfx_leds[4 + (i*9)] = CHSV(gfx_h, gfx_s, gfx_v);
    }
    gfx_v += 5;
    if(gfx_v >= 255){
      gfx_donut_in = false;
      gfx_v = 255;
    }
  }else{
    fill_solid(gfx_leds, NUM_LEDS, CHSV(gfx_h, gfx_s, gfx_v));
    for(int i = 0; i < 6; i++){
      gfx_leds[4 + (i*9)] = CHSV(gfx_h, gfx_s, gfx_v);
    }
    gfx_v -= 5;
    if(gfx_v <= 0){
      gfx_donut_in = true;
      gfx_v = 0;
    }
  }
  FastLED.show();
  gfx_cycleColor();
  FastLED.clearData();
}

void gfx_cycleColor(){
  //if(staticMode == false){
    gfx_h++;
    if(gfx_h >= 255){
      gfx_h = 0;
    }
  //}
}

const byte gfx_snake_route[54] = {0,1,2,3,4,5,6,7,8,11,10,9,14,13,12,17,16,15,47,46,45,50,49,48,53,52,51,24,23,18,19,20,21,22,25,26,27,32,33,34,31,28,29,30,35,36,41,42,43,44,39,40,37,38};

uint8_t gfx_snake_currentLed = 0;

void gfx_snake(){
  if(gfx_snake_currentLed == 0){
    gfx_leds[gfx_snake_route[gfx_snake_currentLed]] = CHSV(gfx_h, gfx_s, gfx_v);
  }else{
    gfx_leds[gfx_snake_route[gfx_snake_currentLed]] = CHSV(gfx_h, gfx_s, gfx_v);
    gfx_leds[gfx_snake_route[gfx_snake_currentLed-1]] = CHSV(gfx_h, gfx_s, gfx_v);
  }
    
  gfx_snake_currentLed++;
  if(gfx_snake_currentLed == NUM_LEDS){
    gfx_snake_currentLed = 0;
  }
  FastLED.show();
  gfx_cycleColor();
  /*currentColor++;
  if(currentColor == 255){
    currentColor = 0;
  }*/
  FastLED.clearData();
}

bool gfx_snakePlus_neg = false;
uint8_t gfx_snakePlus_ledsOn = 2;
//uint8_t gfx_snakePlus_currentLed = 0;

void gfx_snakePlus(){
  if(gfx_snakePlus_neg == false){
    for(int i = 0; i <= gfx_snakePlus_ledsOn; i++){
      if(gfx_snake_currentLed - i < 0){
        byte buf = (gfx_snake_currentLed - i) + 54;
        gfx_leds[gfx_snake_route[buf]] = CHSV(gfx_h, gfx_s, gfx_v);
      }else{
        gfx_leds[gfx_snake_route[gfx_snake_currentLed - i]] = CHSV(gfx_h, gfx_s, gfx_v);
      }    
    }
    gfx_snake_currentLed++;
    if(gfx_snake_currentLed % 9 == 0){gfx_snakePlus_ledsOn++;} //if passed a side +1 led lit
    if(gfx_snakePlus_ledsOn == 54){gfx_snakePlus_neg = true;} //if all leds lit decrease
  }else{
     for(int i = 0; i <= gfx_snakePlus_ledsOn; i++){
      if(gfx_snake_currentLed - i < 0) {
        byte buf = (gfx_snake_currentLed - i) +54;
        gfx_leds[gfx_snake_route[buf]] = CHSV(gfx_h, gfx_s, gfx_v);
      } else {
        gfx_leds[gfx_snake_route[gfx_snake_currentLed - i]] = CHSV(gfx_h, gfx_s, gfx_v);
      }
    }
    gfx_snake_currentLed++;
    if(gfx_snake_currentLed % 9 == 0){gfx_snakePlus_ledsOn--;}
    if(gfx_snakePlus_ledsOn == 1){gfx_snakePlus_neg = false;}
  }
  if(gfx_snake_currentLed == NUM_LEDS){
    gfx_snake_currentLed = 0;
  }
  FastLED.show();
  /*currentColor++;
  if(currentColor == 255){currentColor = 0;}*/
  gfx_cycleColor();
  FastLED.clearData();
}


//int gfx_color = 0;

void gfx_rainbow(){
  fill_solid(gfx_leds, 54, CHSV(gfx_h,255,200));
  delay(1);
  FastLED.show();
  gfx_h++;
  if (gfx_h == 256)
  {
    gfx_h = 1;
  }
}

//make it delayless
void gfx_blink(){
  gfx_leds[8] = CHSV(0,255,255);
  FastLED.show();
  delay(250);
  gfx_leds[8] = CHSV(0,255,0);
  FastLED.show();
  delay(250);
}

const uint8_t gfx_load[8] = {0,1,2,3,8,7,6,5};

void gfx_loading(uint8_t leds, bool stageTwo){
  for(int i = 0; i < leds; i++){
    if(!stageTwo){
      gfx_leds[gfx_load[i]] =  CHSV(170, 200, 80);
    }else{
      gfx_leds[gfx_load[i]] =  CHSV(0, 255, 80);
    }
  }
  FastLED.show();
  FastLED.clearData(); //might be useless
}

void gfx_reset() {
  //leave h alone maybe add these in setup?
  gfx_s = 255;
  gfx_v = 255;
}


