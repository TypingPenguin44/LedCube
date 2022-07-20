#include <defs.h>
#include <FastLED.h>
CRGB gfx_leds[NUM_LEDS] = {0};  // zero initialize array.



void gfx_setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(gfx_leds, NUM_LEDS);
}


int gfx_color = 0;
void gfx_rainbow() {
  fill_solid(gfx_leds, 54, CHSV(gfx_color,255,200));
  delay(1);
  FastLED.show();
  gfx_color++;
  if (gfx_color == 256)
  {
    gfx_color = 1;
  }
}

void gfx_blink()
{
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