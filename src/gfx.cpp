#include <defs.h>
#include <gfx.h>
#define FASTLED_ALLOW_INTERRUPTS 0
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
        gfx_diagonal();
        break;
      case 5:
        gfx_dpad();
        break;
      case 8:
      gfx_dice();
        break;
      case 9:
        gfx_charge();
        break;
      default:
        //implement yellow loading lol
        break;
    }
  }
}
const uint8_t gfx_diagonal_route[54][2] {{35, 38}, //0
                                         {0, 0}, //1
                                         {26, 29}, //2
                                         {0, 0}, //3
                                         {0, 0}, //4
                                         {0, 0}, //5
                                         {42, 9}, //6
                                         {0, 0}, //7
                                         {11, 20}, //8
                                         {6, 42}, //9
                                         {0, 0}, //10
                                         {20, 8}, //11
                                         {0, 0}, //12
                                         {0, 0}, //13
                                         {0, 0}, //14
                                         {42, 47}, //15
                                         {0, 0}, //16
                                         {53, 18}, //17
                                         {17, 53}, //18
                                         {0, 0}, //19
                                         {8, 11}, //20
                                         {0, 0}, //21
                                         {0, 0}, //22
                                         {0, 0}, //23
                                         {51, 27}, //24
                                         {0, 0}, //25
                                         {29, 2}, //26
                                         {24, 51}, //27
                                         {0, 0}, //28
                                         {2, 26}, //29
                                         {0, 0}, //30
                                         {0, 0}, //31
                                         {0, 0}, //32
                                         {45, 36}, //33
                                         {0, 0}, //34
                                         {38, 0}, //35
                                         {33, 45}, //36
                                         {0, 0}, //37
                                         {0, 35}, //38
                                         {0, 0}, //39
                                         {0, 0}, //40
                                         {0, 0}, //41
                                         {47, 15}, //42
                                         {0, 0}, //43
                                         {9, 6}, //44
                                         {36, 33}, //45
                                         {0, 0}, //46
                                         {15, 42}, //47
                                         {0, 0}, //48
                                         {0, 0}, //49
                                         {0, 0}, //50
                                         {27, 24}, //51
                                         {0, 0}, //52
                                         {18, 17} //53
};
const uint8_t gfx_diagonal_helper[4] = {0, 2, 6, 8};
bool gfx_diagonal_first = true; //needs to be reset when anim called again put in gfx reset
bool gfx_diagonal_sideStart = true;
int gfx_diagonal_start = 0;
int gfx_diagonal_opCount = 0;
int gfx_diagonal_current = 0;
int gfx_diagonal_leds[3] = {0};

void gfx_diagonal(){
  if(gfx_diagonal_sideStart){
    gfx_diagonal_sideStart = false;

    if(gfx_diagonal_first){
      gfx_diagonal_first = false;
      gfx_diagonal_start = gfx_diagonal_helper[random(0, 4)]; //select a stating led
      for(int i = 0; i < 3; i++){
        gfx_diagonal_leds[i] = gfx_diagonal_start;
      }
    }
  
    gfx_diagonal_leds[0] = gfx_diagonal_start; // set current led as start
    //Serial.print("start: ");
    //Serial.println(gfx_diagonal_current);

  }else if(gfx_diagonal_opCount != 2){ //else if because need to show first led as well
    gfx_diagonal_opCount++; //count how many times it iterated

    gfx_diagonal_leds[2] = gfx_diagonal_leds[1];
    gfx_diagonal_leds[1] = gfx_diagonal_leds[0];

    //decide wich direction next led should be
    if(gfx_diagonal_start % 9 == 0){
      gfx_diagonal_leds[0] += 4;
    }else if(gfx_diagonal_start % 9 == 2){
      gfx_diagonal_leds[0] += 2;
    } else if(gfx_diagonal_start % 9 == 6){
      gfx_diagonal_leds[0] -= 2;
    } else if(gfx_diagonal_start % 9 == 8){
      gfx_diagonal_leds[0] -= 4;
    }
    //Serial.print("op: ");
    //Serial.print(gfx_diagonal_opCount);
    //Serial.print(" led: ");
    //Serial.println(gfx_diagonal_current);
  }else{
    //Serial.println("New side");
    //pick new side
    gfx_diagonal_sideStart = true;
    gfx_diagonal_opCount = 0;

    int next = random(0, 2);
    //Serial.print("random: ");
    //Serial.println(next);
    gfx_diagonal_start = gfx_diagonal_route[gfx_diagonal_leds[0]][next];
    //Serial.print("newstrat: ");
    //Serial.println(gfx_diagonal_start);

    //one cycle missing here, it will pause here it aint really visible 
    //solution is to break this up into more functions
  }
  //ligth it up
  gfx_leds[gfx_diagonal_leds[2]] = CHSV(gfx_h, 255, 100);
  gfx_leds[gfx_diagonal_leds[1]] = CHSV(gfx_h, 255, 170);
  gfx_leds[gfx_diagonal_leds[0]] = CHSV(gfx_h, 255, 255);
  gfx_cycleColor();
  FastLED.show();
  FastLED.clearData();
}
int gfx_dpad_cornerVal = 0;
int gfx_dpad_centerVal = 0;
int gfx_dpad_sideVal = 0;
bool gfx_dpad_sideNeg = false;
bool gfx_dpad_explode = false; //if i wanna transition smoother

void gfx_dpad(){
  if(gfx_dpad_explode){

    for(int i = 0; i < 6; i++){ //6 sides of cube
      gfx_leds[1 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_sideVal);
      gfx_leds[3 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_sideVal);
      gfx_leds[5 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_sideVal);
      gfx_leds[7 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_sideVal);

      gfx_leds[4 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_centerVal);
    }
    FastLED.show();
    FastLED.clearData();

    gfx_dpad_sideVal -= 4;
    if(gfx_dpad_sideVal <= 0){
      gfx_dpad_sideVal = 0;
    }

    gfx_dpad_centerVal -= 2;
    if(gfx_dpad_centerVal <= 0){
      gfx_dpad_explode = false;
      gfx_dpad_cornerVal = 0;
      gfx_cycleColor(20);
    }
  }else{
    for(int i = 0; i < 6; i++){ //6 sides of cube
      gfx_leds[0 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[2 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[6 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[8 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
    }
    FastLED.show();
    FastLED.clearData();
  
    gfx_dpad_cornerVal += 2;
    if(gfx_dpad_cornerVal >= 255){
      gfx_dpad_explode = true;
      gfx_dpad_centerVal = 255;
      gfx_dpad_sideVal = 255;
      gfx_cycleColor(20);
    }
  }
}


/*int gfx_dpad_cornerVal = 255;
int gfx_dpad_centerVal = 0;
int gfx_dpad_sideVal = 0;
bool gfx_dpad_sideNeg = false;
bool gfx_dpad_explode = false; //if i wanna transition smoother
void gfx_dpad(){
  if(gfx_dpad_explode == false){
    for(int i = 0; i < 6; i++){ //6 sides of cube
      gfx_leds[0 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[2 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[6 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[8 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);

      gfx_leds[4 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_centerVal);
    }
    FastLED.show();
    //cleardata?
    FastLED.clearData();
    gfx_dpad_cornerVal--;
    gfx_dpad_centerVal++;
    if(gfx_dpad_centerVal == 255){
      gfx_dpad_explode = true;
      gfx_dpad_cornerVal = 0;
    }
  }else{
    for(int i = 0; i < 6; i++){ //6 sides of cube
      gfx_leds[0 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[2 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[6 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);
      gfx_leds[8 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_cornerVal);

      gfx_leds[1 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_sideVal);
      gfx_leds[3 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_sideVal);
      gfx_leds[5 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_sideVal);
      gfx_leds[7 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_sideVal);

      gfx_leds[4 + i * 9] = CHSV(gfx_h, 255, gfx_dpad_centerVal);
    }
    FastLED.show();
    //cleardata?
    FastLED.clearData();
    if(gfx_dpad_sideNeg){
      gfx_dpad_sideVal -= 7;
    }else{
      gfx_dpad_sideVal += 7;
    }
       
    gfx_dpad_cornerVal += 2;
    gfx_dpad_centerVal -= 2;
    if(gfx_dpad_sideVal >= 255){
      gfx_dpad_sideNeg = true;
      gfx_dpad_sideVal = 255;
    }else if(gfx_dpad_sideVal <= 0){
      gfx_dpad_sideVal = 0;
    }
    if(gfx_dpad_cornerVal >= 255){
      gfx_dpad_cornerVal = 255;
      gfx_dpad_centerVal = 0;
      gfx_dpad_explode = false;
      gfx_dpad_sideNeg = false;
    }
  }
}*/

void gfx_charge(){
  BATTERY = analogRead(A0);
  gfx_h = map(BATTERY, 637, 860, 0, 95);
  gfx_leds[4] = CHSV(gfx_h, 255, 50);
  FastLED.show();
}

//sensor should be called separately 
uint8_t gfx_dice_side = 0;
void gfx_dice(){
  switch(gfx_dice_side){
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
      gfx_leds[4 + (i*9)] = CHSV(0, 0, 0);
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
    gfx_h += 1;
    if(gfx_h >= 255){
      gfx_h = 0;
    }
  //}
}

void gfx_cycleColor(int a){
  //if(staticMode == false){
    gfx_h += a;
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

void gfx_reset(){
  //leave h alone maybe add these in setup?
  gfx_diagonal_first = true;
  gfx_s = 255;
  gfx_v = 255;
}
void gfx_clear(){
  FastLED.clearData();
  FastLED.show();
}


