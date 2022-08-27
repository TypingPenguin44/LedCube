#include <NeoPixelBus.h>
/*
* Function prototypes for gfx 
*/

/**
 * @brief calls strip begin and initializes leds
 */
void gfx_setup();
/**
 * @brief resets frequently changing values to default to make every
 * animation to play nice
 */
void gfx_reset();
/**
 * @brief "decides" which animations to call with appropriate timing
 */
void gfx_animHandler();
/**
 * @brief cycles colors for animations  
 */
void gfx_cycleColor();
/**
 * @brief cycles colors for animations
 * @param a value to increment by
 */
void gfx_cycleColor(float a);
/**
 * @brief clears leds, show nothing
 */
void gfx_clear();

//animations
void gfx_snake();
void gfx_rainbow();
void gfx_loading(uint8_t leds, bool stageTwo);
void gfx_loading_anim();
void gfx_snakePlus();
void gfx_donut();
void gfx_dice();
void gfx_dice_set();
void gfx_charge();
void gfx_dpad(); //replace this with a freefall reactive anim?
void gfx_diagonal();
void gfx_fill(HsbColor color);
void gfx_lines();
void gfx_bubble(); //last standalone anim hopefully still need some extra 
void gfx_fade();
void gfx_static();

//startup fade in or out
void gfx_startFade(bool error = false, bool in = true);

//only used in low battery poweroff, it didnt work 100% when tested
void gfx_blink();