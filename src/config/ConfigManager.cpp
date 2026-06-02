#include "ConfigManager.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

static const char* CONFIG_PATH = "/config.json";

ConfigManager& ConfigManager::instance() {
  static ConfigManager mgr;
  return mgr;
}

void ConfigManager::begin() {
  if (!LittleFS.begin()) {
    Serial.println("ConfigManager: LittleFS mount failed");
  }
}

Config ConfigManager::load() {
  Config c;
  if (!LittleFS.begin()) {
    Serial.println("ConfigManager: LittleFS mount failed on load");
    return c;
  }

  if (!LittleFS.exists(CONFIG_PATH)) {
    return c;
  }

  File file = LittleFS.open(CONFIG_PATH, "r");
  if (!file) {
    Serial.println("ConfigManager: failed to open config file");
    return c;
  }

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();
  if (error) {
    Serial.printf("ConfigManager: JSON parse failed %s\n", error.c_str());
    return c;
  }

  c.ssid = doc["ssid"].as<const char*>();
  c.password = doc["password"].as<const char*>();
  c.city = doc["city"].as<const char*>();
  c.latitude = doc["latitude"] | 0.0;
  c.longitude = doc["longitude"] | 0.0;
  c.sleepStartHour = doc["sleepStartHour"].as<int>();
  c.sleepEndHour = doc["sleepEndHour"].as<int>();
  return c;
}

void ConfigManager::save(const Config& cfg) {
  if (!LittleFS.begin()) {
    Serial.println("ConfigManager: LittleFS mount failed on save");
    return;
  }

  StaticJsonDocument<512> doc;
  doc["ssid"] = cfg.ssid;
  doc["password"] = cfg.password;
  doc["city"] = cfg.city;
  doc["latitude"] = cfg.latitude;
  doc["longitude"] = cfg.longitude;
  doc["sleepStartHour"] = cfg.sleepStartHour;
  doc["sleepEndHour"] = cfg.sleepEndHour;

  File file = LittleFS.open(CONFIG_PATH, "w");
  if (!file) {
    Serial.println("ConfigManager: failed to open config file for writing");
    return;
  }
  serializeJson(doc, file);
  file.close();
}
