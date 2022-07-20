#include <Arduino.h>
#include <FastLED.h>

#include <Adafruit_ADXL345_U.h>

//#include <betterDelay.h>
#include <network.h>
#include <button.h>
#include <defs.h>
#include <gfx.h>




CRGB leds[54];
//Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);



extern long currentTime;// = millis();
long currentTime;

/*TODO
lambda fucntions in index html

test Wifi.setOutputPower

network.h get a variable for ssid and pass

specify number of retries initializing?

add "tags" before debug messages 

button.h
the button held down while forever loops can crash the system
delay in button holding? add timer?

set pins and define globals!
*/

uint8_t MODE = 0;
extern int gfx_color;

void setup() {
  //initialize pins
  pinMode(latch, OUTPUT);
  digitalWrite(latch, HIGH); // keep the device on

  pinMode(BTN, INPUT);

  Serial.begin(74880); // open serial
  //wirelessSetup();
  gfx_setup();

  //initServer();
  Serial.println(MODE);

}

void loop() {
  gfx_rainbow();
  inputCheck();
  //delay(30);

}