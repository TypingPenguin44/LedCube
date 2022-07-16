#ifndef BUTTON_H
#define BUTTON_H


#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__"\n")
#else
#define DEBUG_MSG(...)
#endif

bool single_press = false;
bool double_press = false;
bool long_press = false;

void startupReset();
void inputCheck();





#endif
