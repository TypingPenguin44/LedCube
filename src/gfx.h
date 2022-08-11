#include <NeoPixelBus.h>
/*
* Function prototypes for gfx 
*/
void gfx_setup();
void gfx_reset();
void gfx_animHandler();
void gfx_cycleColor();
void gfx_cycleColor(float a);
void gfx_clear();

void gfx_snake();
void gfx_rainbow();
void gfx_loading(uint8_t leds, bool stageTwo);
void gfx_snakePlus();
void gfx_donut();
void gfx_dice();
void gfx_charge();
void gfx_dpad(); //replace this with a freefall reactive anim?
void gfx_diagonal();
void gfx_fill(HsbColor color);
void gfx_lines();
void gfx_bubble(); //last standalone anim hopefully still need some extra 

int gfx_startFade(bool error = false);
