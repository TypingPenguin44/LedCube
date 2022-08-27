#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @brief initializes spiffs
 */
void settings_setup();
/**
 * @brief shutdown sequence
 */
void settings_shutdown();

//same ideas as toggles, different values
void settings_save_gfx();
void settings_load_gfx();
void settings_reset_gfx();


void settings_save_toggles();
/**
 * @brief put default values in the variables then save it 
 * with the save function
 */
void settings_reset_toggles();
void settings_load_toggles();

//calls the 2 load and reset functions
void settings_reset();
void settings_load();

#endif