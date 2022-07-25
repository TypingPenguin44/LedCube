#include <Arduino.h>
#include <FastLED.h>

#include <network.h>
#include <button.h>
#include <defs.h>
#include <gfx.h>
#include <settings.h>
#include <time.h>

int current_anim = 0;

animations gfx[10];
/*anim

diagonal line pulling trail traversing sides 
*/

/*TODO
lambda fucntions in index html

test Wifi.setOutputPower

network.h get a variable for ssid and pass

specify number of retries initializing?

add "tags" before debug messages 

button.h
the button held down while forever loops can crash the system
delay in button holding? add timer?

set pins and define globals!.

shomehow define NOT JUST DECALRE variables logically lol

global settings from phone to turn off and on shake to cycle animations == shake sets single press true in certain modes


maybve figure out a way to reorder the animations... switch cases need constant values smh
if else is best for this but idk if this feature is useful at all


check accel on startup 

maybe proper debug messages ? eg: DEBUG_WIFI("[APConfig] local_ip: %s gateway: %s subnet: %s\n", local_ip.toString().c_str(), gateway.toString().c_str(), subnet.toString().c_str());

do i need to stop spiffs?

on button press reset static thingy

implement toggles

mode switching and sstatic_colors thingy make uniform and not dumb

implement off and stuff
still artifacting....... damned fastled lib
 it was somethign with clear or cleardata or smthn


 startup reset, the toggles save and read and stuff
 HANDLING ADXL ANIM!

 make the turnoff thingy

 static mdoe, switch out all chsv(xyz) thginys to actually have the hsv variables
*/
bool shakeCycle = false;
int BATTERY = 0;
uint8_t MODE = 0;

void setup() {
  pinMode(latch, OUTPUT);
  pinMode(BTN, INPUT);
  digitalWrite(latch, HIGH); // keep the device on

  Serial.begin(74880); // open serial
  
  network_stop(); //wifi off

  settings_setup(); //setup spiffs

  gfx_setup(); //init gfx

  button_startReset(); //check for startup reset

  settings_load_gfx(); //read config from file
  //maybe load other toggles here aswell

  button_accelSetup(); //init adxl

  network_setup(); //init wifi
  network_initServer(); //init server

  current_anim = 0;
}

void loop(){
  yield(); //its good to have it here idk if its needed
  
  /*if (shakeCycle || gfx[current_anim].adxl){
    button_sensorRead();
  }*/ //kinda should rethink this currently it only modofies sides var it aint got nothing to do with shake detect
  gfx_animHandler();


  if(time_test(2000)){
    Serial.print("Mode: ");
    Serial.print(MODE);
    Serial.print(" Anim: ");
    Serial.println(current_anim);
  }
  button_check();
}