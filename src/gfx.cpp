#include <defs.h>
#include <gfx.h>

#include <NeoPixelBus.h>
#include <timers.h>

const uint16_t PixelCount = 54; 
const uint8_t PixelPin = 2; //rx pin
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);


float gfx_h = 0.0;
float gfx_s = 1.0;
float gfx_v = 1.0;

void gfx_setup() {
  strip.Begin();
}

void gfx_animHandler(){
  if(MODE != 2){
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
        case 6:
          gfx_dice();
          break;
        case 7:
          gfx_bubble();
          break;
        case 8:
          gfx_loading_anim();
          break;
        case 9:
          gfx_lines();
          break;
        case 10:
          gfx_charge();
          break;
        case 11:
          gfx_fade();
          break;
        case 12:
          gfx_static();
          break;
        default:
          //implement yellow loading
          current_anim = 0;
          break;
      }
    }
  }else if (MODE == 2 && time_clear(2000)){
    gfx_clear();
  }
  
}

//cant think of a better perfoming method, lots of unused memory tho
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
    }else if(gfx_diagonal_start % 9 == 6){
      gfx_diagonal_leds[0] -= 2;
    }else if(gfx_diagonal_start % 9 == 8){
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
  strip.SetPixelColor(gfx_diagonal_leds[2], HsbColor(gfx_h, gfx_s, 0.4));
  strip.SetPixelColor(gfx_diagonal_leds[1], HsbColor(gfx_h, gfx_s, 0.6));
  strip.SetPixelColor(gfx_diagonal_leds[0], HsbColor(gfx_h, gfx_s, 1));
  
  gfx_cycleColor();
  strip.Show();
  strip.ClearTo(0);
}
float gfx_dpad_cornerVal = 0.0;
float gfx_dpad_centerVal = 0.0;
float gfx_dpad_sideVal = 0.0;
bool gfx_dpad_sideNeg = false;
bool gfx_dpad_explode = false; //if i wanna transition smoother

void gfx_dpad(){
  //Serial.println("what");
  if(gfx_dpad_explode){
    //Serial.println("exp");
    //Serial.println(String(gfx_h) + " " + String(gfx_s) + " " + String(gfx_dpad_sideVal));
    for(int i = 0; i < 6; i++){ //6 sides of cube
      strip.SetPixelColor(1 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_sideVal));
      strip.SetPixelColor(3 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_sideVal));
      strip.SetPixelColor(5 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_sideVal));
      strip.SetPixelColor(7 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_sideVal));

      strip.SetPixelColor(4 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_centerVal));
    }
    strip.Show();
    strip.ClearTo(0);

    gfx_dpad_sideVal -= 0.016;
    if(gfx_dpad_sideVal <= 0){
      gfx_dpad_sideVal = 0;
    }

    gfx_dpad_centerVal -= 0.008;
    if(gfx_dpad_centerVal <= 0){
      gfx_dpad_explode = false;
      gfx_dpad_cornerVal = 0;
      gfx_cycleColor(0.08);
    }
  }else{
    //Serial.println("else");
    //Serial.println(String(gfx_h) + " " + String(gfx_s) + " " + String(gfx_dpad_cornerVal));
    for(int i = 0; i < 6; i++){ //6 sides of cube
      strip.SetPixelColor(0 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_cornerVal));
      strip.SetPixelColor(2 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_cornerVal));
      strip.SetPixelColor(6 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_cornerVal));
      strip.SetPixelColor(8 + i * 9, HsbColor(gfx_h, gfx_s, gfx_dpad_cornerVal));
    }
    strip.Show();
    strip.ClearTo(0);
  
    gfx_dpad_cornerVal += 0.008;
    if(gfx_dpad_cornerVal >= 1){
      gfx_dpad_explode = true;
      gfx_dpad_centerVal = 1;
      gfx_dpad_sideVal = 1;
      gfx_cycleColor(0.08);
    }
  }
}

void gfx_charge(){
  BATTERY = analogRead(A0);
  int val = map(BATTERY, 637, 860, 0, 370);
  strip.SetPixelColor(4, HsbColor(val / 1000, 1, 0.5));
  strip.Show();
}

//sensor should be called separately 
int gfx_dice_side = 0;

bool gfx_dice_scramble = false;
int dice_side_offset = 0; // for scramble thingy
int gfx_scramble_length = 0;
//int gfx_scramble_values[15] = {0};
int gfx_scramble_progress = 0;
bool gfx_dice_freeze = false;
int even = 1;

void gfx_dice(){
  if(gfx_dice_scramble && even % 2 == 0){ //&& time_scramble()){ //useless if time <50 ms 
    strip.ClearTo(0);
    dice_side_offset = random(0,5);
    for(int i = 0; i < 6; i++){
      gfx_dice_side = i;
      gfx_dice_set();
    }
    gfx_scramble_progress++;
    if(gfx_scramble_progress >= gfx_scramble_length){
      gfx_dice_scramble = false;
      gfx_dice_freeze = true;
      time_freeze(); //set the prev value
      gfx_scramble_progress = 0;
      dice_side_offset = 0;
    }
  }else if(gfx_dice_freeze && time_freeze()){
    gfx_dice_freeze = false;
  }else if(!gfx_dice_scramble && !gfx_dice_freeze){
    strip.ClearTo(0);
    gfx_dice_set();
  }
  even++;
  if(even == 3){
    even = 1;
  }
  gfx_cycleColor();
  strip.Show();
}
void gfx_dice_set(){
  switch(gfx_dice_side){
    case 0:
      strip.SetPixelColor((4 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 3:
      strip.SetPixelColor((29 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((33 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 4:
      strip.SetPixelColor((38 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((40 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((42 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 2:
      strip.SetPixelColor((18 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((20 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((24 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((26 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 1:
      strip.SetPixelColor((9 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((11 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((13 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((15 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((17 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 5:
      strip.SetPixelColor((45 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((46 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((47 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((51 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((52 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor((53 + dice_side_offset * 9) % 54, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
  }
}

bool gfx_donut_in = false;
void gfx_donut(){
  if(gfx_donut_in == true){
    //currentColor++;
    for(int i = 0; i < 6; i++){
      strip.SetPixelColor(4 + (i*9), HsbColor(gfx_h, gfx_s, gfx_v));
    }
    gfx_v += 0.02;
    if(gfx_v >= 1){
      gfx_donut_in = false;
      gfx_v = 1;
    }
  }else{
    gfx_fill(HsbColor(gfx_h, gfx_s, gfx_v));
    
    for(int i = 0; i < 6; i++){
      strip.SetPixelColor(4 + (i*9), HsbColor(0, 0, 0));
    }
    gfx_v -= 0.02;
    if(gfx_v <= 0){
      gfx_donut_in = true;
      gfx_v = 0;
    }
  }
  strip.Show();
  gfx_cycleColor();
  strip.ClearTo(0);
}
void gfx_cycleColor(){
  if(static_colors == false){
    gfx_h += 0.004;
    if(gfx_h >= 1){
      gfx_h = 0;
    }
  }
}

void gfx_cycleColor(float a){
  if(static_colors == false){
    gfx_h += a;
    if(gfx_h >= 1){
      gfx_h = 0;
    }
  }
}

const byte gfx_snake_route[54] = {0,1,2,3,4,5,6,7,8,11,10,9,14,13,12,17,16,15,47,46,45,50,49,48,53,52,51,24,23,18,19,20,21,22,25,26,27,32,33,34,31,28,29,30,35,36,41,42,43,44,39,40,37,38};

uint8_t gfx_snake_currentLed = 0;

void gfx_snake(){
  if(gfx_snake_currentLed == 0){
    strip.SetPixelColor(gfx_snake_route[gfx_snake_currentLed], HsbColor(gfx_h, gfx_s, gfx_v));
  }else{
    strip.SetPixelColor(gfx_snake_route[gfx_snake_currentLed], HsbColor(gfx_h, gfx_s, gfx_v));
    strip.SetPixelColor(gfx_snake_route[gfx_snake_currentLed-1], HsbColor(gfx_h, gfx_s, gfx_v));
  }
    
  gfx_snake_currentLed++;
  if(gfx_snake_currentLed == NUM_LEDS){
    gfx_snake_currentLed = 0;
  }
  strip.Show();
  gfx_cycleColor();
  strip.ClearTo(0);
}

bool gfx_snakePlus_neg = false;
uint8_t gfx_snakePlus_ledsOn = 2;
//uint8_t gfx_snakePlus_currentLed = 0;

void gfx_snakePlus(){
  if(gfx_snakePlus_neg == false){
    for(int i = 0; i <= gfx_snakePlus_ledsOn; i++){
      if(gfx_snake_currentLed - i < 0){
        byte buf = (gfx_snake_currentLed - i) + 54;
        strip.SetPixelColor(gfx_snake_route[buf], HsbColor(gfx_h, gfx_s, gfx_v));
      }else{
        strip.SetPixelColor(gfx_snake_route[gfx_snake_currentLed - i], HsbColor(gfx_h, gfx_s, gfx_v));
      }    
    }
    gfx_snake_currentLed++;
    if(gfx_snake_currentLed % 9 == 0){gfx_snakePlus_ledsOn++;} //if passed a side +1 led lit
    if(gfx_snakePlus_ledsOn == 54){gfx_snakePlus_neg = true;} //if all leds lit decrease
  }else{
     for(int i = 0; i <= gfx_snakePlus_ledsOn; i++){
      if(gfx_snake_currentLed - i < 0) {
        byte buf = (gfx_snake_currentLed - i) +54;
         strip.SetPixelColor(gfx_snake_route[buf], HsbColor(gfx_h, gfx_s, gfx_v));
      } else {
        strip.SetPixelColor(gfx_snake_route[gfx_snake_currentLed - i], HsbColor(gfx_h, gfx_s, gfx_v));
      }
    }
    gfx_snake_currentLed++;
    if(gfx_snake_currentLed % 9 == 0){gfx_snakePlus_ledsOn--;}
    if(gfx_snakePlus_ledsOn == 1){gfx_snakePlus_neg = false;}
  }
  if(gfx_snake_currentLed == NUM_LEDS){
    gfx_snake_currentLed = 0;
  }
  strip.Show();
  gfx_cycleColor();
  strip.ClearTo(0);
}

void gfx_rainbow(){
  gfx_fill(HsbColor(gfx_h, 1, 1));
  
  strip.Show();
  gfx_h += 0.004;
  if(gfx_h >= 1)
  {
    gfx_h = 0;
  }
}

//make it delayless
void gfx_blink(){
  strip.ClearTo(0);
  strip.Show();
  strip.SetPixelColor(8, HsbColor(0, 1, 1));
  strip.Show();
  delay(250);
  strip.SetPixelColor(8, HsbColor(0, 0, 0));
  strip.Show();
  delay(250);
}

const uint8_t gfx_load[8] = {0,1,2,3,8,7,6,5};
bool gfx_loading_clkwise = true;
int gfx_loading_count = 0;
int loadingbuf = 0;

void gfx_loading_anim(){
  if(gfx_loading_clkwise){
    for(int i = 0; i < 3; i++){

      if(gfx_loading_count - i < 0){
        loadingbuf = gfx_load[(9 - i)];
        loadingbuf += (gfx_dice_side*9);
      }else{
        loadingbuf = gfx_load[gfx_loading_count - i];
        loadingbuf += (gfx_dice_side*9);
      }
      //Serial.println("count: " + String(gfx_loading_count) + " buf " + String(loadingbuf));
      if(i == 0){
        strip.SetPixelColor(loadingbuf, HsbColor(gfx_h, gfx_s, gfx_v));
      }else if(i == 1){
        strip.SetPixelColor(loadingbuf, HsbColor(gfx_h, gfx_s, gfx_v*0.75));
      }else if(i == 2){
        strip.SetPixelColor(loadingbuf, HsbColor(gfx_h, gfx_s, gfx_v*0.5));
      }
    }
    gfx_loading_count++;
    if(gfx_loading_count >= 8){
      gfx_loading_count = 0;
    }
  }else{
    for(int i = 0; i < 3; i++){
      if(gfx_loading_count + i > 7){
        loadingbuf = gfx_load[(gfx_loading_count + i)%8];
        loadingbuf += (gfx_dice_side*9);
      }else{
        loadingbuf = gfx_load[gfx_loading_count + i];
        loadingbuf += (gfx_dice_side*9);
      }
      //Serial.println("count: " + String(gfx_loading_count) + " buf " + String(loadingbuf));          
      if(i == 0){
        strip.SetPixelColor(loadingbuf, HsbColor(gfx_h, gfx_s, gfx_v));
      }else if(i == 1){
        strip.SetPixelColor(loadingbuf, HsbColor(gfx_h, gfx_s, gfx_v*0.75));
      }else if(i == 2){
        strip.SetPixelColor(loadingbuf, HsbColor(gfx_h, gfx_s, gfx_v*0.5));
      }
    }
    gfx_loading_count--;
    if(gfx_loading_count <= 0){
      gfx_loading_count = 8;
    }
  }
  strip.Show();
  strip.ClearTo(0); //might be useless
  gfx_cycleColor();
}

void gfx_loading(uint8_t leds, bool stageTwo){
  for(int i = 0; i < leds; i++){
    if(!stageTwo){
      strip.SetPixelColor(gfx_load[i], HsbColor(0.55, 0.6, 0.4));
    }else{
      strip.SetPixelColor(gfx_load[i], HsbColor(0, 1, 0.4));
    }
  }
  strip.Show();
  strip.ClearTo(0); //might be useless
}

void gfx_reset(){
  //leave h alone maybe add these in setup?
  gfx_diagonal_first = true;
  gfx_s = 1;
  gfx_v = 1;
}
void gfx_clear(){
  strip.ClearTo(0);
  strip.Show();
}
void gfx_fill(HsbColor color){
  for(int i = 0; i < PixelCount; i++){
    strip.SetPixelColor(i, color);
  }
}
int gfx_lines_roll[3] = {0};
/**
 * @brief stores the path for lines anim
 * [0-2][] x axis
 * [3-5][] y axis
 * [5-8][] z axis
 * its less "if else" if its one single array instead of 3 separate
 */
int gfx_lines_path[9][12] = {{0, 5, 6, 9, 14, 15, 47, 46, 45, 33, 34, 35},
                             {1, 4, 7, 10, 13, 16, 48, 49, 50, 32, 31, 30},
                             {2, 3, 8, 11, 12, 17, 53, 52, 51, 27, 28, 29},
                             {0, 1, 2, 26, 25, 24, 51, 50, 45, 36, 37, 38},
                             {5, 4, 3, 21, 22, 23, 52, 49, 46, 41, 40, 39},
                             {6, 7, 8, 20, 19, 18, 53, 48, 47, 42, 43, 44},
                             {9, 10, 11, 20, 21, 26, 29, 30, 35, 38, 39, 44},
                             {14, 13, 12, 19, 22, 25, 28, 31, 34, 37, 40, 43},
                             {15, 16, 17, 18, 23, 24, 27, 32, 33, 36, 41, 42}};

int gfx_lines_current = 0;
int gfx_lines_led = 0;
int gfx_lines_axis = 0;

void gfx_lines(){
  /*Serial.print(gfx_lines_axis);
  Serial.print(" ");
  Serial.println(gfx_lines_roll[gfx_lines_axis]);*/
  
  for(int i = 0; i < 9; i++){
    strip.SetPixelColor(gfx_lines_path[gfx_lines_axis * 3 + gfx_lines_roll[gfx_lines_axis]][(gfx_lines_led + i) % 12], HsbColor(gfx_h, gfx_s, gfx_v));
  }
  strip.Show();
  strip.ClearTo(0);
  
  gfx_lines_led++;
  if(gfx_lines_led >= 12){
    gfx_lines_led = 0;
  }
  gfx_cycleColor();
}

/**
 * @brief helper array for storing wich corner is connected by which edge 
 * to which corner
 */
int gfx_bubble_edges_helper[8][8] = {{0, 0, 0, 3, 4, 0, 0, 0},
                                     {0, 0, 1, 0, 0, 5, 0, 0},
                                     {0, 1, 0, 2, 0, 0, 6, 0},
                                     {3, 0, 2, 0, 0, 0, 0, 7},
                                     {4, 0, 0, 0, 0, 8, 0, 11},
                                     {0, 5, 0, 0, 8, 0, 9, 0},
                                     {0, 0, 6, 0, 0, 9, 0, 10},
                                     {0, 0, 0, 7, 11, 0, 10, 0}};

int gfx_bubble_edges[12][2] = {{1, 30},
                               {3, 21},
                               {7, 10},
                               {5, 39},
                               {34, 37},
                               {25, 28},
                               {12, 19},
                               {14, 43},
                               {32, 50},
                               {23, 52},
                               {16, 48},
                               {41, 45}}; 

int gfx_bubble_corners[8][3] = {{0, 38, 35}, //top left
                                 {2, 26, 29},
                                 {8, 11, 20},
                                 {6, 44, 9},
                                 {45, 36, 33}, //under top left
                                 {51, 27, 24},
                                 {53, 18, 17},
                                 {47, 15, 42}};
                                 
int gfx_bubble_corner = 0;
bool gfx_bubble_transition = false;
int gfx_bubble_fromto[2] = {0}; 
bool middlereise = true;
bool equal = false;
bool middledecr = false;

float gfx_bubble_from_v = 1.0;
float gfx_bubble_middle_v = 0.0;
float gfx_bubble_to_v = 0.0;

bool middleup = true;
bool middledown = false;

void gfx_bubble(){
  if(gfx_bubble_transition){
    for(int i = 0; i < 3; i++){
      strip.SetPixelColor(gfx_bubble_corners[gfx_bubble_fromto[0]][i], HsbColor(gfx_h, gfx_s, gfx_bubble_from_v));
    }
    for(int i = 0; i < 2; i++){
      strip.SetPixelColor(gfx_bubble_edges[gfx_bubble_edges_helper[gfx_bubble_fromto[0]][gfx_bubble_fromto[1]]][i], HsbColor(gfx_h, gfx_s, gfx_bubble_middle_v));
    }
    for(int i = 0; i < 3; i++){
      strip.SetPixelColor(gfx_bubble_corners[gfx_bubble_fromto[1]][i], HsbColor(gfx_h, gfx_s, gfx_bubble_to_v));
    }

    if(middleup){
      gfx_bubble_from_v -= 0.18; //0.06
      gfx_bubble_middle_v += 0.12; //0.04
      gfx_bubble_to_v += 0.12; //0.02
    }
    if(gfx_bubble_from_v <= 0.0){
      middleup = false;
      gfx_bubble_from_v = 0.0;
      middledown = true;
    }
    if(middledown){
      gfx_bubble_middle_v -= 0.12; //0.04
      gfx_bubble_to_v += 0.12; //0.04
    }
    if(gfx_bubble_middle_v <= 0 || gfx_bubble_to_v >= 1){
      middledown = false;
      gfx_bubble_from_v = 1.0;
      gfx_bubble_middle_v = 0.0;
      gfx_bubble_to_v = 0.0;
      gfx_bubble_transition = false;
      middleup = true; // reset value for next transtion
      
      gfx_bubble_fromto[0] = gfx_bubble_fromto[1];
    }
    //Serial.println("fromv: " + String(gfx_bubble_from_v) + " middelv: " + String(gfx_bubble_middle_v) + " tov: " + String(gfx_bubble_to_v));

  }else{
    for(int i = 0; i < 3; i++){
      strip.SetPixelColor(gfx_bubble_corners[gfx_bubble_fromto[0]][i], HsbColor(gfx_h, gfx_s, 1)); // this is useless in theory
    }
  }

  strip.Show();
  strip.ClearTo(0);
  gfx_cycleColor();
}

float gfx_startFade_value = 0;
void gfx_startFade(bool error, bool in){
  if(error){
    gfx_h = 0;
  }else{
    gfx_h = 0.65;
  }

  if(in){
    gfx_startFade_value = 0;
    while(!(gfx_startFade_value >= 0.5)){
      //yield();
      gfx_fill(HsbColor(gfx_h, 1, gfx_startFade_value));
      strip.Show();
      gfx_startFade_value += 0.01;
      delay(5);
    }
  }else{
    gfx_startFade_value = 1; //= gfx_v?????
    while(!(gfx_startFade_value <= 0)){
      //yield();
      gfx_fill(HsbColor(gfx_h, 1, gfx_startFade_value));
      strip.Show();
      gfx_startFade_value -= 0.01;
      if(gfx_startFade_value < 0){
        gfx_startFade_value = 0;
      }
      delay(5);
    }
  } 
}

bool gfx_fade_in = false;
void gfx_fade(){
  gfx_fill(HsbColor(gfx_h, gfx_s, gfx_v));
  //Serial.println(String(gfx_h) + " " + String(gfx_s) + " " + String(gfx_v));
  strip.Show();
  if(!gfx_fade_in){
    gfx_v -= 0.012;
    if(gfx_v <= 0){
      gfx_fade_in = true;
      gfx_v = 0; //prevent out of bounds
    }
  }else{
    gfx_v += 0.012;
    if(gfx_v >= 1){
      gfx_fade_in = false;
      gfx_v = 1; //prevent out of bounds
    }
  }
}
void gfx_static(){
  gfx_fill(HsbColor(gfx_h, gfx_s, gfx_v));
  //Serial.println(String(gfx_h) + " " + String(gfx_s) + " " + String(gfx_v));
  strip.Show();
}

