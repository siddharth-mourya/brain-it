#pragma once
#include <Arduino.h>

namespace deskbot {

struct AppConfig {
  String ssid;
  String password;
  String city = "Desk";
  float latitude = 28.6139F;
  float longitude = 77.2090F;
  String timezone = "IST-5:30";
  int sleepStartHour = 23;
  int sleepEndHour = 7;
  uint8_t brightness = 180;
  String personality = "friendly";
};

class ConfigManager {
 public:
  bool begin();
  bool load();
  bool save();
  bool save(const AppConfig& config);
  void resetToDefaults();
  bool hasWifiCredentials() const;
  AppConfig& config() { return config_; }
  const AppConfig& config() const { return config_; }
  const char* lastError() const { return lastError_.c_str(); }

 private:
  static constexpr const char* kConfigPath = "/config.json";
  AppConfig config_{};
  String lastError_;
};

}  // namespace deskbot
