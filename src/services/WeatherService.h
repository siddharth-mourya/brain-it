#pragma once
#include <Arduino.h>
#include "../config/ConfigManager.h"
#include "../core/EventBus.h"

namespace deskbot {

struct WeatherSnapshot {
  bool valid = false;
  float temperatureC = 0;
  float windKph = 0;
  int code = 0;
  String summary = "Loading";
  uint32_t updatedAt = 0;
};

class WeatherService {
 public:
  void begin(EventBus& bus, const ConfigManager& config);
  void tick(uint32_t now);
  const WeatherSnapshot& snapshot() const { return weather_; }
  bool fetchNow();

 private:
  EventBus* bus_ = nullptr;
  const ConfigManager* config_ = nullptr;
  WeatherSnapshot weather_{};
  uint32_t lastFetchMs_ = 0;
  String summaryForCode(int code) const;
};

}  // namespace deskbot
