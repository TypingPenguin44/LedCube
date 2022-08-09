#include <defs.h>
#include <gfx.h>

#include <NeoPixelBus.h>
#include <time.h>

const uint16_t PixelCount = 54; 
const uint8_t PixelPin = 2; //rx pin
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

float gfx_h = 0.0;
float gfx_s = 1.0;
float gfx_v = 1.0;

void gfx_setup() {
  //FastLED.addLeds<WS2812B, LED_PIN, GRB>(gfx_leds, NUM_LEDS);
  strip.Begin();
}

void gfx_animHandler(){
  //strip.ClearTo(0); //maybe needed idk
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
      case 7:
        gfx_dice();
        break;
      case 8:
        gfx_lines();
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

//cant think of a better perfoming method, lots of unused memory
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
  strip.SetPixelColor(gfx_diagonal_leds[2], HsbColor(gfx_h, gfx_s, 0.4));
  strip.SetPixelColor(gfx_diagonal_leds[1], HsbColor(gfx_h, gfx_s, 0.6));
  strip.SetPixelColor(gfx_diagonal_leds[0], HsbColor(gfx_h, gfx_s, 1));
  
  gfx_cycleColor();
  strip.Show();
  strip.ClearTo(0);
}
int gfx_dpad_cornerVal = 0;
int gfx_dpad_centerVal = 0;
int gfx_dpad_sideVal = 0;
bool gfx_dpad_sideNeg = false;
bool gfx_dpad_explode = false; //if i wanna transition smoother

void gfx_dpad(){
  if(gfx_dpad_explode){

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
      gfx_cycleColor(20);
    }
  }else{
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
    strip.Show();
    //cleardata?
    strip.ClearTo(0);
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
    strip.Show();
    //cleardata?
    strip.ClearTo(0);
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
  int val = map(BATTERY, 637, 860, 0, 370);
  strip.SetPixelColor(4, HsbColor(val / 1000, 1, 0.5));
  strip.Show();
}

//sensor should be called separately 
uint8_t gfx_dice_side = 0;
void gfx_dice(){
  switch(gfx_dice_side){
    case 1:
      strip.SetPixelColor(4, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 2:
      strip.SetPixelColor(29, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(33, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 3:
      strip.SetPixelColor(38, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(40, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(42, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 4:
      strip.SetPixelColor(18, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(20, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(24, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(26, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 5:
      strip.SetPixelColor(9, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(11, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(13, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(15, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(17, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
    case 6:
      strip.SetPixelColor(45, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(46, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(47, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(51, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(52, HsbColor(gfx_h, gfx_s, gfx_v));
      strip.SetPixelColor(53, HsbColor(gfx_h, gfx_s, gfx_v));
    break;
  }
  gfx_cycleColor();
  strip.Show();
  strip.ClearTo(0);
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
  /*currentColor++;
  if(currentColor == 255){
    currentColor = 0;
  }*/
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
  /*currentColor++;
  if(currentColor == 255){currentColor = 0;}*/
  gfx_cycleColor();
  strip.ClearTo(0);
}


//int gfx_color = 0;

void gfx_rainbow(){
  gfx_fill(HsbColor(gfx_h, 1, 1));
  
  strip.Show();
  gfx_h += 0.004;
  if (gfx_h >= 1)
  {
    gfx_h = 0;
  }
}

//make it delayless
void gfx_blink(){
  strip.SetPixelColor(8, HsbColor(0, 1, 1));
  strip.Show();
  delay(250);
  strip.SetPixelColor(8, HsbColor(0, 0, 0));
  strip.Show();
  delay(250);
}

const uint8_t gfx_load[8] = {0,1,2,3,8,7,6,5};

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
int gfx_x_roll = 0;
int gfx_y_roll = 0;
int gfx_z_roll = 0;
/**
 * @brief stores the path for lines anim
 * [0-2][] x axis
 * [3-5][] y axis
 * [5-8][] z axis
 * its less if else if its one single array instead of 3
 */
int gfx_lines_path[9][12] = {{0, 5, 6, 9, 14, 15, 47, 46, 45, 33, 34, 35},
                             {1, 4, 7, 10, 13, 16, 48, 49, 50, 32, 31, 30},
                             {2, 3, 8, 11, 12, 17, 53, 52, 51, 27, 28, 29},
                             {0, 1, 2, 26, 25, 24, 51, 50, 45, 36, 37, 38},
                             {5, 4, 3, 21, 22, 23, 52, 49, 45, 41, 40, 39},
                             {6, 7, 8, 20, 19, 18, 53, 48, 47, 42, 43, 44},
                             {9, 10, 11, 20, 21, 26, 29, 30, 35, 38, 39, 44},
                             {14, 13, 12, 19, 22, 25, 28, 31, 34, 37, 40, 43},
                             {15, 16, 17, 18, 23, 24, 27, 32, 33, 36, 41, 42}};

int gfx_lines_current = 0;
int linesbuf = 0;
void gfx_lines(){

  Serial.println(gfx_x_roll);
  strip.ClearTo(0);
  strip.SetPixelColor(gfx_lines_path[gfx_lines_current * 3 + gfx_x_roll][linesbuf], HsbColor(gfx_h, 1, 1));
  strip.Show();
  linesbuf++;
  if(linesbuf >= 12){
    linesbuf = 0;
  }
  gfx_cycleColor();
}

