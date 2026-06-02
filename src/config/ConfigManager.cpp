#include "ConfigManager.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

namespace deskbot {

bool ConfigManager::begin() {
  if (!LittleFS.begin(true)) {
    lastError_ = "LittleFS mount failed";
    return false;
  }
  return load();
}

bool ConfigManager::load() {
  if (!LittleFS.exists(kConfigPath)) {
    resetToDefaults();
    return save();
  }

  File file = LittleFS.open(kConfigPath, "r");
  if (!file) {
    lastError_ = "Config open failed";
    resetToDefaults();
    return false;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();
  if (error) {
    lastError_ = String("Config JSON error: ") + error.c_str();
    resetToDefaults();
    return false;
  }

  config_.ssid = doc["ssid"] | "";
  config_.password = doc["password"] | "";
  config_.city = doc["city"] | "Desk";
  config_.latitude = doc["latitude"] | 28.6139F;
  config_.longitude = doc["longitude"] | 77.2090F;
  config_.timezone = doc["timezone"] | "IST-5:30";
  config_.sleepStartHour = constrain(doc["sleepStartHour"] | 23, 0, 23);
  config_.sleepEndHour = constrain(doc["sleepEndHour"] | 7, 0, 23);
  config_.brightness = constrain(doc["brightness"] | 180, 0, 255);
  config_.personality = doc["personality"] | "friendly";
  lastError_ = "";
  return true;
}

bool ConfigManager::save() { return save(config_); }

bool ConfigManager::save(const AppConfig& config) {
  JsonDocument doc;
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["city"] = config.city;
  doc["latitude"] = config.latitude;
  doc["longitude"] = config.longitude;
  doc["timezone"] = config.timezone;
  doc["sleepStartHour"] = config.sleepStartHour;
  doc["sleepEndHour"] = config.sleepEndHour;
  doc["brightness"] = config.brightness;
  doc["personality"] = config.personality;

  File file = LittleFS.open(kConfigPath, "w");
  if (!file) {
    lastError_ = "Config write open failed";
    return false;
  }
  bool ok = serializeJsonPretty(doc, file) > 0;
  file.close();
  if (ok) {
    config_ = config;
    lastError_ = "";
  } else {
    lastError_ = "Config write failed";
  }
  return ok;
}

void ConfigManager::resetToDefaults() { config_ = AppConfig{}; }

bool ConfigManager::hasWifiCredentials() const { return config_.ssid.length() > 0; }

}  // namespace deskbot
