#include <Arduino.h>
#include <network.h>


#define latch 14 // pin # of latch that keeps the device on
#define BTN 12 // pin # of button

/*TODO
lambda fucntions in index html

test Wifi.setOutputPower



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
  //WiFi.setOutputPower(10); //20.5 max 0 lowest
  /*Serial.print("Setting soft-AP configuration ... "); //Wifi setup
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("LedCube", "OooShiny") ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());*/



}

void loop() {
  // put your main code here, to run repeatedly:
}