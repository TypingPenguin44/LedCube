#include <Arduino.h>
//#include <NeoPixelBus.h> //using this library but had to modify

#include <network.h>
#include <io.h>
#include <defs.h>
#include <gfx.h>
#include <settings.h>
#include <time.h>

animations gfx[10];


/*TODO
add "tags" before debug messages 

maybe proper debug messages ? eg: DEBUG_WIFI("[APConfig] local_ip: %s gateway: %s subnet: %s\n", local_ip.toString().c_str(), gateway.toString().c_str(), subnet.toString().c_str());

on button press reset static thingy

static colors test

add diagonal option to html
donut anim too fast color change

startup fade in
shutdown fade out in mode 0 and 1

documentation 
charging check every 5 sec
auto poweroff 
toggles in web interface 
test every button on web 
charge number on web should be % not random integer between 470 and 600 something
clear out serial.println and replace (the useful ones) with debug msg 

in loop move conditions into respective functions
*/
bool shakeCycle = true;
int BATTERY = 0;
uint8_t MODE = 0;
int current_anim = 0;
bool static_colors = false;


void printconfig(){
  for (int i = 0; i < 10; i++){
    Serial.print(gfx[i].interval);
    Serial.print(" ");
    Serial.println(gfx[i].adxl);
  }
}

uint8_t ISR_press_count = 0;

ICACHE_RAM_ATTR void off() {
  if(ISR_press_count == 3){
    Serial.println("power off");
    settings_shutdown();
    
  }else{
    ISR_press_count++;
    time_ISR_prev = millis();
  }
}


void setup() {
  
  pinMode(latch, OUTPUT);
  pinMode(BTN, INPUT);
  digitalWrite(latch, HIGH); // keep the device on

  Serial.begin(74880); // open serial
  
  
  network_stop(); //wifi off
  Serial.println("wifi off");

  settings_setup(); //setup spiffs
  Serial.println("fs setup");

  gfx_setup(); //init gfx
  Serial.println("init gfx");

  io_startReset(); //check for startup reset
  Serial.println("startup reset");

  settings_load_gfx(); //read config from file
  //maybe load other toggles here aswell
  Serial.println("load gfx");
  Serial.println("config");
  printconfig();
  Serial.println();

  io_accelSetup(); //init adxl
  Serial.println("setup adxl");

  network_setup(); //init wifi
  network_initServer(); //init server  
}

void loop(){
  yield(); //its good to have it here idk if its needed

  if(time_ISR() && ISR_press_count != 0){
    ISR_press_count = 0;
    Serial.println("Isr Count Reset");
  }
  
  if (shakeCycle || gfx[current_anim].adxl){
    io_sensorRead();
  } //kinda should rethink this currently it only modofies sides var it aint got nothing to do with shake detect
  /*if(gfx[current_anim].adxl){
    button_sensorRead(); //timer already in function
    //Serial.println("yo");
  }*/
  if(MODE != 2){
    gfx_animHandler();
  }else if (MODE == 2 && time_clear(2000)){
    //lil bit dumb way to clear leds when in mode 2 and playign with loading anim
    //should be moved to animhandler
    gfx_clear();
  }
  
  if(time_test(2000)){
    Serial.print("Mode: ");
    Serial.print(MODE);
    Serial.print(" Anim: ");
    Serial.print(current_anim);
    Serial.print(" side: ");
    Serial.println(gfx_dice_side);
  }
  io_check();
}