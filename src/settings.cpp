#include <ArduinoJson.h>
#include <FS.h>
#include <defs.h>

static const animations default_gfx[10] = {{0, 40, 0}, //rainbow
                                           {0, 60, 0}, //sanke 
                                           {0, 40, 0}, //snake++
                                           {0, 20, 0}, //donut
                                           {0, 60, 0}, //diagonal
                                           {0, 20, 0}, //dpad?
                                           {0, 50, 1}, //bubble
                                           {0, 50, 1}, //dice
                                           {0, 60, 1}, //lines
                                           {0, 2000, 0}}; //charge

void settings_setup(){
  if(!SPIFFS.begin()){ //spiffs setup
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void settings_shutdown(){
  SPIFFS.end();
  Serial.end();
  digitalWrite(latch, LOW);
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
  for(int i = 0; i < 10; i++){
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
    return;
  }

  for (int i = 0; i < 10; i++){
    JsonObject item = doc["gfx"][i];
    gfx[i].interval = item["interval"];
    gfx[i].adxl = item["adxl"];
  }

  file.close();
}

void settings_reset_delays(){
  for(int i = 0; i < 10; i++){
    gfx[i].interval = default_gfx[i].interval;
    gfx[i].adxl = default_gfx[i].adxl; //u cant change this lol 
  }
  settings_save_gfx();
}

void settings_reset(){
  
}