#ifndef NETWORK_H
#define NETWORK_H

#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif


#include <Arduino.h>
#include <ESP8266WiFi.h>

int wirelessSetup();

IPAddress local_IP(192,168,4,2);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

int wirelessSetup()
{
  DEBUG_MSG("Setting soft-AP configuration ... "); //Wifi setup
  DEBUG_MSG(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  DEBUG_MSG("Setting soft-AP ... ");
  DEBUG_MSG(WiFi.softAP("LedCube", "OooShiny") ? "Ready" : "Failed!");
}


#endif