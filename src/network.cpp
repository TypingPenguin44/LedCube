#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <defs.h>
#include <settings.h>
#include <gfx.h>

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
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_AP);
  DEBUG_MSG("Setting soft-AP configuration ...\n");
  DEBUG_MSG(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready\n" : "Failed!\n");
  DEBUG_MSG("Setting soft-AP ... \n");
  DEBUG_MSG(WiFi.softAP("LedCube", "OooShiny") ? "Ready\n" : "Failed!\n");
}

void network_stop(){
  DEBUG_MSG("[Network] Turning off wifi");
  WiFi.mode(WIFI_OFF);
}
void network_poweroff(){
  server.end();
  DEBUG_MSG("[Network] Turning off wifi");
  WiFi.mode(WIFI_OFF);
}

void network_initServer()
{
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "404 Not found");
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/gfx", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/gfx_settings.json", String(), false);
  });
  server.on("/toggles", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/gfx_toggles.json", String(), false);
  });
  server.on("/static", HTTP_GET, [](AsyncWebServerRequest *request){
    current_anim = request->getParam("id")->value().toInt();
    static_colors = true;
    gfx_h = request->getParam("h")->value().toFloat();
    gfx_s = request->getParam("s")->value().toFloat();
    gfx_v = request->getParam("v")->value().toFloat();
    request->send(200);
  });
  server.on("/colors", HTTP_GET, [](AsyncWebServerRequest *request){
    //set last trigger time to 0 so instant color change?
    if(static_colors){
      gfx_h = request->getParam("h")->value().toFloat();
      gfx_s = request->getParam("s")->value().toFloat();
      gfx_v = request->getParam("v")->value().toFloat();
      //Serial.println(String(gfx_h) + " " + String(gfx_s) + " " + String(gfx_v));
    }
    request->send(200);
  });

  server.on("/anim", HTTP_GET, [](AsyncWebServerRequest *request){
    current_anim = request->getParam("id")->value().toInt();
    gfx_reset();
    Serial.println(current_anim);
    static_colors = false;
    request->send(200);
  });
  //sends battery charge value to control page
  server.on("/getPercent", HTTP_GET, [](AsyncWebServerRequest *request){
    //AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", String(battery));
    //response->addHeader("Server","ESP Async Web Server");
    //request->send(response);
  });
  server.on("/press", HTTP_GET, [](AsyncWebServerRequest *request){
    single_press = true;
    static_colors = false;
    gfx_reset();
    request->send(200);
  });
  server.on("/doublepress", HTTP_GET, [](AsyncWebServerRequest *request){
    double_press = true;
    static_colors = false;
    //Serial.println()
    gfx_reset();
    request->send(200);
  });
  server.on("/longpress", HTTP_GET, [](AsyncWebServerRequest *request){
    long_press = true;
    static_colors = false;
    gfx_reset();
    request->send(200);
  });
  server.on("/delay", HTTP_GET, [](AsyncWebServerRequest *request){
    gfx[current_anim].interval = request->getParam("val")->value().toInt();
    request->send(200);
  });

  server.on("/resetdelay", HTTP_GET, [](AsyncWebServerRequest *request){
    settings_reset_delays();
    request->send(200);
  });
  server.on("/savedelay", HTTP_GET, [](AsyncWebServerRequest *request){
    settings_save_gfx();
    request->send(200);
  });
  server.on("/saveconfig", HTTP_GET, [](AsyncWebServerRequest *request){
    settings_save_gfx();
    settings_save_toggles();
    request->send(200);
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    settings_shutdown();
    request->send(200);
  });
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });
  server.on("/reload", HTTP_GET, [](AsyncWebServerRequest *request){
    settings_load_gfx();
    request->send(200);
  });
  server.on("/charge", HTTP_GET, [](AsyncWebServerRequest *request){
    sleep_press = true;
    request->send(200);
  });
  server.on("/enable", HTTP_GET, [](AsyncWebServerRequest *request){
    int id = request->getParam("id")->value().toInt();
    toggles[id] = true;
    request->send(200);
  });
  server.on("/disable", HTTP_GET, [](AsyncWebServerRequest *request){
    int id = request->getParam("id")->value().toInt();
    toggles[id] = false;
    request->send(200);
  });
  server.on("/shake", HTTP_GET, [](AsyncWebServerRequest *request){
    shake = true;
    Serial.println("shaketrue");
    delay(2);
    request->send(200);
  });
  server.begin();
}
