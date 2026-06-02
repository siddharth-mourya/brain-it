#include "WeatherService.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

namespace deskbot {

void WeatherService::begin(EventBus& bus, const ConfigManager& config) {
  bus_ = &bus;
  config_ = &config;
}

void WeatherService::tick(uint32_t now) {
  if (WiFi.status() != WL_CONNECTED) return;
  if (lastFetchMs_ == 0 || now - lastFetchMs_ > 15UL * 60UL * 1000UL) fetchNow();
}

bool WeatherService::fetchNow() {
  if (!config_ || WiFi.status() != WL_CONNECTED) return false;
  lastFetchMs_ = millis();
  const AppConfig& cfg = config_->config();
  String url = "https://api.open-meteo.com/v1/forecast?latitude=" + String(cfg.latitude, 4) +
               "&longitude=" + String(cfg.longitude, 4) + "&current_weather=true";
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  http.setTimeout(6000);
  if (!http.begin(client, url)) {
    if (bus_) bus_->emit(EventType::WeatherFailed, 0, "HTTP begin failed");
    return false;
  }
  int status = http.GET();
  if (status != HTTP_CODE_OK) {
    http.end();
    if (bus_) bus_->emit(EventType::WeatherFailed, status, "Weather HTTP failed");
    return false;
  }
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, http.getStream());
  http.end();
  if (err) {
    if (bus_) bus_->emit(EventType::WeatherFailed, 0, err.c_str());
    return false;
  }
  JsonObject current = doc["current_weather"];
  weather_.temperatureC = current["temperature"] | 0.0F;
  weather_.windKph = current["windspeed"] | 0.0F;
  weather_.code = current["weathercode"] | 0;
  weather_.summary = summaryForCode(weather_.code);
  weather_.valid = true;
  weather_.updatedAt = millis();
  if (bus_) bus_->emit(EventType::WeatherUpdated);
  return true;
}

String WeatherService::summaryForCode(int code) const {
  if (code == 0) return "Clear";
  if (code <= 3) return "Cloudy";
  if (code == 45 || code == 48) return "Fog";
  if (code >= 51 && code <= 67) return "Drizzle";
  if (code >= 71 && code <= 77) return "Snow";
  if (code >= 80 && code <= 82) return "Rain";
  if (code >= 95) return "Storm";
  return "Weather";
}

}  // namespace deskbot
