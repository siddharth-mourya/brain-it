#include "WeatherService.h"
#if defined(USE_WEATHER)
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>

void WeatherService::begin(EventBus* bus) { this->bus = bus; }

WeatherInfo WeatherService::fetch(float lat, float lon) {
  WeatherInfo wi;
  String url = String("https://api.open-meteo.com/v1/forecast?latitude=") + String(lat, 6) + "&longitude=" + String(lon, 6) + "&current_weather=true";
  HTTPClient http;
  http.begin(url);
  int code = http.GET();
  if (code == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(2048);
    DeserializationError err = deserializeJson(doc, payload);
    if (!err) {
      if (doc.containsKey("current_weather")) {
        auto &cw = doc["current_weather"];
        wi.temperature = cw["temperature"].as<float>();
        wi.summary = "OK";
        wi.valid = true;
      }
    }
  }
  http.end();
  return wi;
}

#else
void WeatherService::begin(EventBus* bus) { (void)bus; }
WeatherInfo WeatherService::fetch(float lat, float lon) { (void)lat; (void)lon; WeatherInfo w; w.summary = "N/A"; return w; }
#endif
