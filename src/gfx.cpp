#include <defs.h>
#include <FastLED.h>
CRGB gfx_leds[NUM_LEDS] = {0};  // zero initialize array.

int gfx_color = 0;

void gfx_setup() {
  FastLED.addLeds<WS2812B, 13, GRB>(gfx_leds, NUM_LEDS);
}

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
