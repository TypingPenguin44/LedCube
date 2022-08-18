#include <Arduino.h>
//using this library but had to modify it thats why it isnt in platformio.ini
#include <NeoPixelBus.h>

#include <network.h>
#include <io.h>
#include <defs.h>
#include <gfx.h>
#include <settings.h>
#include <time.h>

animations gfx[12];
bool toggles[2];


/*TODO
add "tags" before debug messages 

maybe proper debug messages ? eg: DEBUG_WIFI("[APConfig] local_ip: %s gateway: %s subnet: %s\n", local_ip.toString().c_str(), gateway.toString().c_str(), subnet.toString().c_str());

fix web button charge
fix loading
 
auto poweroff //need value first
charge number on web should be % not random integer between 470 and 600 something


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
bool SHUTDOWN = false;

ICACHE_RAM_ATTR void off() {
  unsigned long time_now = millis();
  if(ISR_press_count == 3){
    Serial.println("power off");
    detachInterrupt(BTN);
    //settings_shutdown();
    SHUTDOWN = true;
    
  }else if(time_now - time_ISR_prev > 140){
    Serial.println(time_now);
    ISR_press_count++;
    time_ISR_prev = time_now;
    Serial.println("setprevisr");
  }
}


void setup() {
  
  pinMode(latch, OUTPUT);
  pinMode(BTN, INPUT);
  digitalWrite(latch, HIGH); // keep the device on

  attachInterrupt(BTN, off, RISING);
  

  Serial.begin(74880); // open serial
  
  
  network_stop(); //wifi off
  Serial.println("wifi off");

  settings_setup(); //setup spiffs
  Serial.println("fs setup");

  gfx_setup(); //init gfx
  Serial.println("init gfx");

  io_startReset(); //check for startup reset
  Serial.println("startup reset");

  settings_load();
  //maybe load other toggles here aswell
  Serial.println("load gfx");
  Serial.println("config");
  printconfig();
  Serial.println();

  io_accelSetup(); //init adxl
  Serial.println("setup adxl");

  network_setup(); //init wifi
  network_initServer(); //init server
  gfx_startFade();
}

void loop(){
  yield(); //its good to have it here idk if its needed

  if(time_ISR() && ISR_press_count != 0){
    ISR_press_count = 0;
    Serial.println("Isr Count Reset");
  }
  io_sensorRead();

  
  gfx_animHandler();
  
  io_batteryCheck(1); //delay in function 
  
  if(time_test(2000)){
    Serial.print("Mode: ");
    Serial.print(MODE);
    Serial.print(" Anim: ");
    Serial.print(current_anim);
    Serial.print(" side: ");
    Serial.println(gfx_dice_side);
  }
  if(SHUTDOWN){
    settings_shutdown();
  }
  io_check();
}