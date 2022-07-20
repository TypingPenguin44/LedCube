#ifndef NETWORK_H
#define NETWORK_H

#ifdef DEBUG_ESP_PORT
  #ifndef DEBUG_MSG
  #define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__)
  #endif
#else
#define DEBUG_MSG(...)
#endif


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

IPAddress local_IP(192,168,4,2);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

void wirelessSetup();
void setPaths();

#endif

void wirelessSetup()
{
  DEBUG_MSG("Setting soft-AP configuration ...\n"); //Wifi setup
  DEBUG_MSG(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready\n" : "Failed!\n");
  DEBUG_MSG("Setting soft-AP ... \n");
  DEBUG_MSG(WiFi.softAP("LedCube", "OooShiny") ? "Ready\n" : "Failed!\n");
}



void initServer()
{
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Learn to type n00b");
    //request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);//, processor);
  });
  server.on("/static", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("staticbutton");
    request->send(200);
  });
  server.on("/rainbow", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("rain");
    
    request->send(200);
  });

  server.on("/adxlStatic", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("adxlstaticbutton");
    
    request->send(200);
  });

  server.on("/adxlRainbow", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("adxlrain");
    
    request->send(200);
  });
  //Switches to MODE 2 (sleep) and clears LEDs, if in sleep it returns back to last MODE 
  server.on("/shake", HTTP_GET, [](AsyncWebServerRequest *request){
   
    request->send(200);
  });
  //sets colors in static mode
  server.on("/staticRefresh", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(200);
    
  });
  //sends battery charge value to control page
  server.on("/getPercent", HTTP_GET, [](AsyncWebServerRequest *request){
   
    request->send(200);
    //AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", String(battery));
    //response->addHeader("Server","ESP Async Web Server");
    //request->send(response);
  });
  server.on("/press", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.on("/doublePress", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.on("/longPress", HTTP_GET, [](AsyncWebServerRequest *request){
   
    request->send(200);
  });
  server.on("/superLong", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(200);
  });
  server.on("/delay", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(200);
  });
  server.on("/resetDelay", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(200);
  });
  server.on("/tempSaveDelay", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(200);
  });
  server.on("/saveAllConfig", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.on("/loadVar", HTTP_GET, [](AsyncWebServerRequest *request){
   
    request->send(200);
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.on("/reload", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.on("/reloadDelays", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.begin();
}
