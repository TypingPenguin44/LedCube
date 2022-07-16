#include <Arduino.h>
#include <FastLED.h>

#include <Adafruit_ADXL345_U.h>

#include <network.h>


#define latch 14 // pin # of latch that keeps the device on
#define BTN 12 // pin # of button


CRGB leds[54];
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


long currentTime = millis();


/*TODO
lambda fucntions in index html

test Wifi.setOutputPower

network.h get a variable for ssid and pass

specify number of retries initializing?

add "tags" before debug messages 

*/

/*
STRUCTURE

*/

void setup() {
  //initialize pins
  pinMode(latch, OUTPUT);
  digitalWrite(latch, HIGH); // keep the device on

  pinMode(BTN, INPUT);

  Serial.begin(74880); // open serial
  wirelessSetup();

  initServer();
  

}

void loop() {
  // put your main code here, to run repeatedly:
}