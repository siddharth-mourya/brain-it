#include "WeatherPage.h"
#include <Arduino.h>

void WeatherPage::render(Renderer& renderer) {
  if (!svc) { renderer.drawText(0,0,"Weather: N/A"); return; }
  WeatherInfo wi = svc->fetch(lat, lon);
  char buf[64];
  if (wi.valid) {
    snprintf(buf, sizeof(buf), "Weather: %s %.1fC", wi.summary.c_str(), wi.temperature);
  } else {
    snprintf(buf, sizeof(buf), "Weather: unavailable");
  }
  renderer.drawText(0, 0, buf);
}
