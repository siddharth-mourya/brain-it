#pragma once
#include "IPage.h"
#include "../services/WeatherService.h"

class WeatherPage : public IPage {
public:
  WeatherPage(WeatherService* ws, float lat=0.0, float lon=0.0) : svc(ws), lat(lat), lon(lon) {}
  void init() override {}
  void update() override {}
  void render(Renderer& renderer) override;
  const char* name() const override { return "Weather"; }
private:
  WeatherService* svc;
  float lat;
  float lon;
};
