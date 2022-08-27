#include <defs.h>
#include <Arduino.h>
#include <timers.h>
#include <gfx.h>
#include <io.h>
#include <Adafruit_ADXL345_U.h>
#include <settings.h>


#ifdef DEBUG_ESP_PORT
  #ifndef DEBUG_MSG
  #define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__)
  #endif
#else
#define DEBUG_MSG(...)
#endif

//set all input states false
bool single_press = false;
bool double_press = false;
bool long_press = false;
bool sleep_press = false;
bool shake = false;


Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


void io_check(){
  if(digitalRead(BTN) == HIGH){ // if button is pressed
    if(single_press == false){ // if not btn press set it true
      single_press = true;
    } // if pressed AND pressed again in 250ms register as double press
    else if(single_press == true && time_in_check(DOUBLE_PRESS_TIME)){
      while(digitalRead(BTN) == HIGH){} //if still pressed infinite loop
      double_press = true;
      single_press = false;
      DEBUG_MSG("[io_check] Double press\n");
    }
    time_setPrev(); // set previous btn press to now
  }

  while(digitalRead(BTN) == HIGH){
    int buf = time_map(1000, 900, 0, 0, 8);
    if(buf > 1){ // call loading anim only if there is something to display
      gfx_loading(buf, 0);
    } 
    delay(5);
    if(time_check(LONG_PRESS_TIME)){  //if button held down more than 1s
      long_press = true;
      single_press = false;
      double_press = false;
      
      while(digitalRead(BTN) == HIGH){ //if button still pressed
        int buf = time_map(3000, 3000, 0, 0, 8);
        gfx_loading(buf, 1);
        delay(5);
        
        if(time_check(SLEEP_PRESS_TIME)){ //after 3s of holding exec
          sleep_press = true;
          long_press = false;
          single_press = false;
          double_press = false;
          
          while(digitalRead(BTN) == HIGH){yield();} //infinite loop while still holding
        }
      }
    }
  }
  io_handler();
}

void io_handler(){
  //single press executes after 250ms
  if(single_press == true && time_check(DOUBLE_PRESS_TIME)){    
    single_press = false;
    DEBUG_MSG("[io_check] Single press\n");
    gfx_reset();
    
    if(MODE == 0){ //if in normal mode increment normal anim
      current_anim++;
      if(current_anim >= gfx_normal){ //wrap around
        current_anim = 0;
      }
    }else if (MODE == 1){ //if in double mode increment double anim
      current_anim++;
      if(current_anim > 9){ //wrap around
        current_anim = gfx_normal;
      }
    }
  }else if (double_press == true){
    double_press = false;
    gfx_reset(); //reset animation values

    //change states
    if(MODE == 0){
      MODE = 1;
      gfx_clear();
      current_anim = gfx_normal; // starting anim for accel mdoe 
    }
    else if(MODE == 1){
      MODE = 0;  
      gfx_clear();
      current_anim = 0;
    }    
  }
  if(shake){
    //execute animation specific stuff
    if(current_anim == 9){ //lines
      shake = false;
      gfx_lines_axis++;
      if(gfx_lines_axis >= 3){
        gfx_lines_axis = 0;
      }
    }else if(current_anim == 6){ //dice
      shake = false;
      gfx_dice_scramble = true;
      gfx_scramble_length = random(20,50); //get random length of scramble
      //Serial.print("length: ");
      //Serial.println(gfx_scramble_length);
    }else if(current_anim == 8){ //loading
      shake = false;
      gfx_loading_clkwise = !gfx_loading_clkwise; //change clockwise state
    }else if(toggles[0]){ //if shakecycle just increment (works in diagonal as well)
      shake = false;
      single_press = true;
    }
  }

  if(long_press){
    long_press = false;
    DEBUG_MSG("[io_check] Long press\n");

    //change modes
    if(MODE == 0 || MODE == 1) { //if in animation go to sleep
      gfx_clear();
      MODE = 2;
    }else if(MODE == 2) { //if in sleep mode go back to normal with longpress
      MODE = 0;
    }else if(MODE == 3) { //if in charge mode go back to normal with longpress
      MODE = 0;
      current_anim = 0;
    }
  }
  if(sleep_press){
    sleep_press = false;
    DEBUG_MSG("[io_check] Sleep press\n");
    if(MODE == 0 || MODE == 1 || MODE == 2) { 
      gfx_clear();
      MODE = 3;
      current_anim = 10; //set anim to charge
    }
  }
}

void io_accelSetup(){
  if(!accel.begin()){ //adxl345 setup
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    Serial.println("Disabling sensor...");
  }else{
    accel.setRange(ADXL345_RANGE_8_G); //SetRange 16, 8, 4, 2
    accel.setDataRate(ADXL345_DATARATE_100_HZ);

    //calibration data for cube#1
    //got values from test program 
    accel.writeRegister(ADXL345_REG_OFSX, -4);
    accel.writeRegister(ADXL345_REG_OFSY, -3);
    accel.writeRegister(ADXL345_REG_OFSZ, 0);
  }
}
//shake helper values
float shakeX = 0;
float shakeY = 0;
float shakeZ = 0;
int io_shakeCount = 0;
float io_shakeValues[10][3] = {0};

void io_sensorRead(){
  if(toggles[0] || gfx[current_anim].adxl){ //read sensor if shakecycle true or animation requires it 
    if(time_sensor()){
      //get event
      sensors_event_t event; 
      accel.getEvent(&event);

      //extract to determine side up
      int x = event.acceleration.x;
      int y = event.acceleration.y;
      int z = event.acceleration.z;

      //shakecycle
      if(toggles[0] || gfx[current_anim].adxl){
        //average 10 values of ~200ms 
        if(io_shakeCount == 10){
          shakeX = 0.0;
          shakeY = 0.0;
          shakeZ = 0.0;
          //add together
          for(int i = 0; i < io_shakeCount; i++){
            shakeX += io_shakeValues[i][0];
            shakeY += io_shakeValues[i][1];
            shakeZ += io_shakeValues[i][2];
          }
          //divide them
          shakeX = shakeX/io_shakeCount;
          shakeY = shakeY/io_shakeCount;
          shakeZ = shakeZ/io_shakeCount;
          /*Serial.println("OUT");
          Serial.println(x);
          Serial.println(y);
          Serial.println(z);
          Serial.println();*/

          //if time to check determine if one is above threshold 
          if(time_shake()){
            if(shakeX > 15 || shakeY > 15 || shakeZ > 15){
              shake = true;

              /*Serial.println("shake");
              Serial.println("END");
              Serial.println(shakeX);
              Serial.println(shakeY);
              Serial.println(shakeZ);
              Serial.println();*/
            }
          }
          io_shakeCount = 0;
        }
        //get values 
        io_shakeValues[io_shakeCount][0] = fabsf(event.acceleration.x);
        io_shakeValues[io_shakeCount][1] = fabsf(event.acceleration.y);
        io_shakeValues[io_shakeCount][2] = fabsf(event.acceleration.z);

        /*Serial.println("READINGS");
        Serial.println(fabsf(event.acceleration.x));
        Serial.println(fabsf(event.acceleration.y));
        Serial.println(fabsf(event.acceleration.z));
        Serial.println();*/
        //Serial.println(String(event.acceleration.x) + " " + String(event.acceleration.y) + " " + String(event.acceleration.z));
        io_shakeCount++;
      }
      if(current_anim == 9){ //gfx_lines();
        float roll_helper[3] = {0};
        roll_helper[0] = event.acceleration.x;
        roll_helper[1] = event.acceleration.y;
        roll_helper[2] = event.acceleration.z;

        //determine if cube is leaning to a side 
        for(int i = 0; i < 3; i++){
          if(roll_helper[i] >= 4){
            gfx_lines_roll[i] = 0;
          }else if(roll_helper[i] <= -4){
            gfx_lines_roll[i] = 2;
          }else{
            gfx_lines_roll[i] = 1;
          }
        }
      }else if(current_anim == 7){ //gfx_bubble();
        //which corner is up from accel values
        if(x <= -2 && y <= -2 && z <= -2){
          gfx_bubble_corner = 0;
        }else if(x >= 2 && y <= -2 && z <= -2){
          gfx_bubble_corner = 1;
        }else if(x >= 2 && y >= 2 && z <= -2){
          gfx_bubble_corner = 2;
        }else if(x <= -2 && y >= 2 && z <= -2){
          gfx_bubble_corner = 3;
        }else if(x <= -2 && y <= 2 && z >= 2){
          gfx_bubble_corner = 4;
        }else if(x >= 2 && y <= -2 && z >= 2){
          gfx_bubble_corner = 5;
        }else if(x >= 2 && y >= 2 && z >= 2){
          gfx_bubble_corner = 6;
        }else if(x <= -2 && y >= 2 && z >= 2){
          gfx_bubble_corner = 7;
        }
        //if this new corner is not the previous one init transition
        if(gfx_bubble_corner != gfx_bubble_fromto[0] ){ //&& gfx_bubble_transition == false??
          gfx_bubble_transition = true;
          gfx_bubble_fromto[1] = gfx_bubble_corner;
        }
      }else if(current_anim == 6 || current_anim == 8){ //dice, loading
        //determines which side is up      
        if(z <= -6){
          gfx_dice_side = 0; //1
        }else if(z >= 6){
          gfx_dice_side = 5; //6
        }else if(x <= -6){
          gfx_dice_side = 4; //5
        }else if(x >= 6){
          gfx_dice_side = 2; //2
        }else if(y <= -6){
          gfx_dice_side = 3; //3
        }else if(y >= 6){
          gfx_dice_side = 1; //4
        }
      }
    }
  }
}

void io_startReset(){
  while(digitalRead(BTN) == HIGH) {
    int buf = time_map(3000, 3000, 0, 0, 8);
    gfx_loading(buf, 1);
    delay(20);
    if(time_check(SLEEP_PRESS_TIME)){
      DEBUG_MSG("RESET...\n");
      settings_reset();
      while (digitalRead(BTN) == HIGH){yield();}
    }
  }
}

void io_batteryCheck(bool override){
  if(time_chargeCheck()){
    io_getCharge();
    if(BATTERY < 730 && override == false){
      Serial.println("Low battery...");
      gfx_blink();
      SHUTDOWN = true;
    }
  }
}
//get battery charge value
void io_getCharge(){
  BATTERY = analogRead(A0);
  Serial.println(BATTERY);
}