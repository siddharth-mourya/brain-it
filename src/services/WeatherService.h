#pragma once
#include <Arduino.h>
#include "../core/EventBus.h"

struct WeatherInfo {
  String summary;
  float temperature = 0.0;
  bool valid = false;
};

class WeatherService {
public:
  void begin(EventBus* bus);
  WeatherInfo fetch(float lat, float lon);
private:
  EventBus* bus = nullptr;
};
