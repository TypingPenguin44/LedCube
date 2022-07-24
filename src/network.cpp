#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <defs.h>
#include <settings.h>

#ifdef DEBUG_ESP_PORT
  #ifndef DEBUG_MSG
  #define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__)
  #endif
#else
#define DEBUG_MSG(...)
#endif

AsyncWebServer server(80);

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

void network_setup()
{
  WiFi.mode(WIFI_AP);
  DEBUG_MSG("Setting soft-AP configuration ...\n");
  DEBUG_MSG(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready\n" : "Failed!\n");
  DEBUG_MSG("Setting soft-AP ... \n");
  DEBUG_MSG(WiFi.softAP("LedCube", "OooShiny") ? "Ready\n" : "Failed!\n");
}

void network_initServer()
{
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "404 Not found");
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);//, processor);
    //request->send(SPIFFS, "/gfx_settings.json", String(), false);
  });
  server.on("/static", HTTP_GET, [](AsyncWebServerRequest *request){
    current_anim = request->getParam("id")->value().toInt();
    static_colors = true;
    gfx_h = request->getParam("h")->value().toInt();
    gfx_s = request->getParam("s")->value().toInt();
    gfx_v = request->getParam("v")->value().toInt();
    request->send(200);
  });
  server.on("/colors", HTTP_GET, [](AsyncWebServerRequest *request){
    if(static_colors){
      gfx_h = request->getParam("h")->value().toInt();
      gfx_s = request->getParam("s")->value().toInt();
      gfx_v = request->getParam("v")->value().toInt();
    }
    request->send(200);
  });

  server.on("/anim", HTTP_GET, [](AsyncWebServerRequest *request){
    current_anim = request->getParam("id")->value().toInt();
    static_colors = false;
    request->send(200);
  });

  //Switches to MODE 2 (sleep) and clears LEDs, if in sleep it returns back to last MODE 
  server.on("/shake", HTTP_GET, [](AsyncWebServerRequest *request){
   
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
    single_press = true;
    request->send(200);
  });
  server.on("/doublepress", HTTP_GET, [](AsyncWebServerRequest *request){
    double_press = true;
    request->send(200);
  });
  server.on("/longpress", HTTP_GET, [](AsyncWebServerRequest *request){
    long_press = true;
    request->send(200);
  });
  server.on("/delay", HTTP_GET, [](AsyncWebServerRequest *request){
    gfx[current_anim].interval = request->getParam("val")->value().toInt();
    request->send(200);
  });

  server.on("/resetdelay", HTTP_GET, [](AsyncWebServerRequest *request){
    //use a static cosnt array with all values for reset 
    //then save
    settings_reset_delays();
    request->send(200);
  });
  server.on("/savedelay", HTTP_GET, [](AsyncWebServerRequest *request){
    //save gfx to file
    settings_save_gfx();
    request->send(200);
  });
  server.on("/saveAllConfig", HTTP_GET, [](AsyncWebServerRequest *request){
    //save all configs, other config for shake and roll?
    request->send(200);
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){

    request->send(200);
  });
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.on("/reload", HTTP_GET, [](AsyncWebServerRequest *request){
    settings_load_gfx();
    request->send(200);
  });
  server.begin();
}
