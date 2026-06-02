#pragma once
#include <Arduino.h>

struct Config {
  String ssid;
  String password;
  String city;
  float latitude = 0.0;
  float longitude = 0.0;
  int sleepStartHour = 23;
  int sleepEndHour = 7;
};

class ConfigManager {
public:
  static ConfigManager& instance();
  void begin();
  Config load();
  void save(const Config& cfg);
private:
  ConfigManager() = default;
};
