#include "WifiManager.h"
#include <WiFi.h>

namespace deskbot {

void WifiManager::begin(EventBus& bus, ConfigManager& config) {
  bus_ = &bus;
  config_ = &config;
  WiFi.mode(WIFI_STA);
  if (config_->hasWifiCredentials()) connectStation();
  else startPortal();
}

void WifiManager::tick(uint32_t now) {
  if (portalActive_) server_.handleClient();
  bool isConnected = connected();
  if (isConnected != lastConnected_) {
    lastConnected_ = isConnected;
    if (bus_) bus_->emit(isConnected ? EventType::WifiConnected : EventType::WifiDisconnected);
  }
  if (!portalActive_ && !isConnected && config_ && config_->hasWifiCredentials() && now - lastReconnectMs_ > 30000) {
    lastReconnectMs_ = now;
    connectStation();
  }
}

bool WifiManager::connected() const { return WiFi.status() == WL_CONNECTED; }

String WifiManager::ipText() const {
  if (connected()) return WiFi.localIP().toString();
  if (portalActive_) return WiFi.softAPIP().toString();
  return "offline";
}

void WifiManager::connectStation() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(config_->config().ssid.c_str(), config_->config().password.c_str());
}

void WifiManager::startPortal() {
  portalActive_ = true;
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("DeskBot-Setup");
  server_.on("/", HTTP_GET, [this]() { handleRoot(); });
  server_.on("/save", HTTP_POST, [this]() { handleSave(); });
  server_.begin();
  if (bus_) bus_->emit(EventType::PairingStarted);
}

void WifiManager::handleRoot() { server_.send(200, "text/html", htmlPage()); }

void WifiManager::handleSave() {
  AppConfig cfg = config_->config();
  cfg.ssid = server_.arg("ssid");
  cfg.password = server_.arg("password");
  cfg.city = server_.arg("city");
  cfg.latitude = server_.arg("lat").toFloat();
  cfg.longitude = server_.arg("lon").toFloat();
  cfg.personality = server_.arg("personality");
  cfg.brightness = static_cast<uint8_t>(constrain(server_.arg("brightness").toInt(), 0, 255));
  if (config_->save(cfg)) {
    if (bus_) bus_->emit(EventType::ConfigUpdated);
    server_.send(200, "text/html", "<h1>Saved</h1><p>DeskBot will restart.</p>");
    delay(600);
    ESP.restart();
  } else {
    server_.send(500, "text/plain", config_->lastError());
  }
}

String WifiManager::htmlPage() const {
  const AppConfig& c = config_->config();
  String page = "<!doctype html><html><head><meta name=viewport content='width=device-width,initial-scale=1'>";
  page += "<title>DeskBot Setup</title><style>body{font-family:sans-serif;max-width:520px;margin:30px auto;padding:0 14px}input{width:100%;padding:10px;margin:6px 0 14px}button{padding:12px 18px}</style></head><body>";
  page += "<h1>DeskBot Setup</h1><form method=post action=/save>";
  page += "SSID<input name=ssid value='" + c.ssid + "'>";
  page += "Password<input name=password type=password value='" + c.password + "'>";
  page += "City<input name=city value='" + c.city + "'>";
  page += "Latitude<input name=lat value='" + String(c.latitude, 4) + "'>";
  page += "Longitude<input name=lon value='" + String(c.longitude, 4) + "'>";
  page += "Personality<input name=personality value='" + c.personality + "'>";
  page += "Brightness<input name=brightness type=number min=0 max=255 value='" + String(c.brightness) + "'>";
  page += "<button>Save</button></form></body></html>";
  return page;
}

}  // namespace deskbot
