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
*/
bool shakeCycle = false;
int BATTERY = 0;
uint8_t MODE = 0;

void setup() {
  Serial.begin(74880); // open serial
  //initialize pins
  pinMode(latch, OUTPUT);
  digitalWrite(latch, HIGH); // keep the device on

  pinMode(BTN, INPUT);
  button_accelSetup();
  //settings_fsSetup();




  gfx[0].interval = 40;
  gfx[0].adxl = false;

  gfx[1].interval = 60;
  gfx[1].adxl = false;

  gfx[2].interval = 40;
  gfx[2].adxl = false;

  gfx[3].interval = 20;
  gfx[3].adxl = false;

  gfx[7].interval = 2000;
  gfx[7].adxl = false;

  gfx[9].interval = 50;
  gfx[9].adxl = true;
  current_anim = 0;

  
  /*settings_save();
  settings_load();
  settings_save();*/

  //wirelessSetup();
  gfx_setup();

  //initServer();
  Serial.println(MODE);

}

void loop(){
  yield();
  
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
  //delay(30);

}