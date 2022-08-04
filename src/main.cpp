#include <Arduino.h>
#define FASTLED_ALLOW_INTERRUPTS 0
//#include <NeoPixelBus.h>

#include <network.h>
#include <button.h>
#include <defs.h>
#include <gfx.h>
#include <settings.h>
#include <time.h>

animations gfx[10];


/*TODO
lambda fucntions in index html

test Wifi.setOutputPower

add "tags" before debug messages 

global settings from phone to turn off and on shake to cycle animations == shake sets single press true in certain modes

maybve figure out a way to reorder the animations... switch cases need constant values smh
if else is best for this but idk if this feature is useful at all


maybe proper debug messages ? eg: DEBUG_WIFI("[APConfig] local_ip: %s gateway: %s subnet: %s\n", local_ip.toString().c_str(), gateway.toString().c_str(), subnet.toString().c_str());

do i need to stop spiffs?
SPIFFS.end()


on button press reset static thingy

implement toggles

mode switching and sstatic_colors thingy make uniform and not dumb

 test turnoff

 static mdoe, switch out all chsv(xyz) thginys to actually have the variables
  kinda done 

 get the other led lib from libtest

 wifi still dropping but it stays connected at start
*/
bool shakeCycle = false;
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

  button_startReset(); //check for startup reset
  Serial.println("startup reset");

  settings_load_gfx(); //read config from file
  //maybe load other toggles here aswell
  Serial.println("load gfx");
  Serial.println("config");
  printconfig();
  Serial.println();

  button_accelSetup(); //init adxl
  Serial.println("setup adxl");

  network_setup(); //init wifi
  network_initServer(); //init server
  
}

void loop(){
  Serial.println("fu");
  yield(); //its good to have it here idk if its needed

  if(time_ISR() && ISR_press_count != 0){
    ISR_press_count = 0;
    Serial.println("Isr Count Reset");
  }
  
  /*if (shakeCycle || gfx[current_anim].adxl){
    button_sensorRead();
  }*/ //kinda should rethink this currently it only modofies sides var it aint got nothing to do with shake detect
  if(gfx[current_anim].adxl){
    button_sensorRead(); //timer already in function
    //Serial.println("yo");
  }
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
  button_check();
}