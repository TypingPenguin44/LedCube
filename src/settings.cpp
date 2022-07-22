#include <ArduinoJson.h>
#include <FS.h>
#include <defs.h>

void settings_fsSetup(){
  if(!SPIFFS.begin()){ //spiffs setup
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}
  

void settings_save(){
  SPIFFS.remove("/gfx_settings.json");

  File file = SPIFFS.open("/gfx_settings.json", "w");

  if (!file) {
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
  }
  
  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  // Close the file
  file.close();
}