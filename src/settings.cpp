#include <ArduinoJson.h>
#include <FS.h>
#include <defs.h>
#include <gfx.h>
#include <network.h>

static const animations default_gfx[13] = {{0, 40, 0}, //rainbow
                                           {0, 60, 0}, //sanke 
                                           {0, 40, 0}, //snake++
                                           {0, 20, 0}, //donut
                                           {0, 60, 0}, //diagonal
                                           {0, 10, 0}, //dpad?
                                           {0, 50, 1}, //dice
                                           {0, 20, 1}, //bubble 50
                                           {0, 50, 1}, //loading
                                           {0, 60, 1}, //lines
                                           {0, 2000, 0}, //charge
                                           {0, 50, 0}, //fade
                                           {0, 200, 0}}; //static

static const bool default_toggles[2] = {true, false};

void settings_setup(){
  if(!SPIFFS.begin()){ //spiffs setup
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void settings_shutdown(){
  network_poweroff();
  Serial.println("power off");
  gfx_startFade(0, 0);
  delay(2);
  Serial.flush();
  SPIFFS.end();
  Serial.end();
  
  digitalWrite(latch, LOW);
}
void settings_save_toggles(){
  SPIFFS.remove("/gfx_toggles.json");
  File file = SPIFFS.open("/gfx_toggles.json", "w");
  if(!file){
    Serial.println(F("Failed to create file"));
    return;
  }
  DynamicJsonDocument doc(512);
  for(int i = 0; i < 2; i++){
    doc["toggles"][i] = toggles[i];
  }

  if(serializeJson(doc, file) == 0){
    Serial.println(F("Failed to write to file"));
  }
  // Close the file
  file.close();
}
void settings_load_toggles(){
  File file = SPIFFS.open("/gfx_toggles.json", "r");

  if(!file){
    Serial.println(F("Failed to open file"));
    return;
  }
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, file);

  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    file.close();
    return;
  }

  for (int i = 0; i < 2; i++){
    toggles[i] = doc['toggles'][i].as<bool>(); 
  }
  file.close();
}


void settings_reset_toggles(){
  for(int i = 0; i < 2; i++){
    toggles[i] = default_toggles[i];
  }
  settings_save_toggles();
}
  
void settings_save_gfx(){
  SPIFFS.remove("/gfx_settings.json");

  File file = SPIFFS.open("/gfx_settings.json", "w");

  if(!file){
    Serial.println(F("Failed to create file"));
    return;
  }

  DynamicJsonDocument doc(1024);
  JsonArray arr = doc.createNestedArray("gfx");
  // Set the values in the document
  for(int i = 0; i < NUM_ANIM; i++){
    JsonObject obj = arr.createNestedObject();
    obj["interval"] = gfx[i].interval;
    obj["adxl"] = gfx[i].adxl;
    /*Serial.print(i);
    Serial.print(" ");
    Serial.println(gfx[i].adxl);*/
  }
  
  // Serialize JSON to file
  if (serializeJson(doc, file) == 0){
    Serial.println(F("Failed to write to file"));
  }
  // Close the file
  file.close();
}

void settings_load_gfx(){
  File file = SPIFFS.open("/gfx_settings.json", "r");

  if(!file){
    Serial.println(F("Failed to open file"));
    return;
  }
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, file);

  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    file.close();
    return;
  }

  for (int i = 0; i < NUM_ANIM; i++){
    JsonObject item = doc["gfx"][i];
    gfx[i].interval = item["interval"];
    gfx[i].adxl = item["adxl"];
  }

  file.close();
}

void settings_reset_delays(){
  for(int i = 0; i < NUM_ANIM; i++){
    gfx[i].interval = default_gfx[i].interval;
    gfx[i].adxl = default_gfx[i].adxl; //u cant change this lol 
  }
  settings_save_gfx();
}

void settings_reset(){
  settings_reset_toggles();
  settings_reset_delays();
}

void settings_load(){
  settings_load_gfx();
  settings_load_toggles();
}