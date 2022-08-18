#include <Arduino.h>
//using this library but had to modify it thats why it isnt in platformio.ini
#include <NeoPixelBus.h>

#include <network.h>
#include <io.h>
#include <defs.h>
#include <gfx.h>
#include <settings.h>
#include <timers.h>

animations gfx[NUM_ANIM];
bool toggles[NUM_TOGGLES];

/*TODO
add "tags" before debug messages 

maybe proper debug messages ? eg: DEBUG_WIFI("[APConfig] local_ip: %s gateway: %s subnet: %s\n", local_ip.toString().c_str(), gateway.toString().c_str(), subnet.toString().c_str());

3.5v 730
4.2v 880
1.68A @ 4.2 full white should be 3. something amps....
*/
bool SHUTDOWN = false;
int BATTERY = 0;
int MODE = 0;
int current_anim = 0;
bool static_colors = false;

//prints out config
void printconfig(){
  for (int i = 0; i < NUM_ANIM; i++){
    Serial.print(gfx[i].interval);
    Serial.print(" ");
    Serial.println(gfx[i].adxl);
  }
}

//interrupt based power off function
//3 fast presses turns the cube off
uint8_t ISR_press_count = 0;

//this function gets called every buttonpress
ICACHE_RAM_ATTR void off() { 
  time_now = millis();
  if(ISR_press_count == 3){ //if 3rd press start shutdown 
    Serial.println("Power off");
    //detach interrupt so it wont restart the shutdown
    detachInterrupt(BTN);
    SHUTDOWN = true;

  }else if(time_now - time_ISR_prev > 140){ //if other press in 140ms ingore it
    Serial.println(time_now);
    ISR_press_count++;
    time_ISR_prev = time_now;
    Serial.println("Set prev isr");
  }
}


void setup() {
  //Set latch to output 
  pinMode(latch, OUTPUT);
  pinMode(BTN, INPUT);
  //set latch high so it stays on
  digitalWrite(latch, HIGH);

  //attach interrupt for buttonpresses
  attachInterrupt(BTN, off, RISING);
  
  Serial.begin(74880); //needed for debug
  
  Serial.println("Wifi off");
  network_stop(); //wifi off
  
  Serial.println("Filesystem setup");
  settings_setup(); //setup spiffs
  
  Serial.println("Init gfx");
  gfx_setup();
  
  Serial.println("Startup reset check");
  io_startReset();
  
  Serial.println("Load config");
  settings_load();
  
  //prints config on startup
  //Serial.println("Config");
  //printconfig();
  //Serial.println();

  Serial.println("Setup adxl");
  io_accelSetup();
  
  Serial.println("Init wifi");
  network_setup();

  Serial.println("Start server");
  network_initServer();

  gfx_startFade(); //Startup fade in anim
}

void loop(){
  yield(); //its good to have it here idk if its needed

  //This resets the isr shutdown counter if no buttonpress for 300ms
  if(time_ISR() && ISR_press_count != 0){
    ISR_press_count = 0;
    Serial.println("Isr Count Reset");
  }
  //read adxl values 
  io_sensorRead();

  //render animations
  gfx_animHandler();
  
  //check battery levels
  io_batteryCheck(toggles[1]);
  
  //print mode, current anim, side value
  /*if(time_test(2000)){
    Serial.print("Mode: ");
    Serial.print(MODE);
    Serial.print(" Anim: ");
    Serial.print(current_anim);
    Serial.print(" side: ");
    Serial.println(gfx_dice_side);
  }*/
  //Check buttonpresses
  io_check();

  //If shutdown triggered start shutdown
  if(SHUTDOWN){
    settings_shutdown();
  }
}