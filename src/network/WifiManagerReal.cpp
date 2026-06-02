#include "WifiManagerReal.h"
#if defined(USE_WIFI)
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#else
#include <WiFi.h>
#include <WebServer.h>
#endif
#include <Arduino.h>

#if defined(ESP8266)
static ESP8266WebServer* portalServer = nullptr;
#else
static WebServer* portalServer = nullptr;
#endif

void WifiManagerReal::begin(EventBus* bus) {
  this->bus = bus;
  cfg = ConfigManager::instance().load();
}

void WifiManagerReal::connect() {
  cfg = ConfigManager::instance().load();
  if (cfg.ssid.length() == 0) {
    Serial.println("WifiManager: no stored SSID, starting provisioning portal...");
    startProvisionPortal();
    return;
  }
  connectWithConfig();
}

void WifiManagerReal::connectWithConfig() {
  Serial.printf("WifiManager: connecting to '%s'...\n", cfg.ssid.c_str());
  WiFi.mode(WIFI_STA);
  WiFi.begin(cfg.ssid.c_str(), cfg.password.c_str());
  unsigned long start = millis();
  while (millis() - start < 15000) {
    if (WiFi.status() == WL_CONNECTED) break;
    delay(200);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WifiManager: connected");
    if (bus) bus->emit(EventType::WIFI_CONNECTED);
  } else {
    Serial.println("WifiManager: failed to connect, starting portal");
    startProvisionPortal();
  }
}

bool WifiManagerReal::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}

void handlePortalRoot() {
  String html = "<html><body><h1>DeskBot Provisioning</h1>";
  html += "<form method='POST' action='/save'>";
  html += "SSID: <input name='ssid'><br>Password: <input name='pass' type='password'><br>City: <input name='city'><br>";
  html += "<input type='submit' value='Save'></form></body></html>";
  portalServer->send(200, "text/html", html);
}

void handlePortalSave() {
  if (!portalServer) return;
  String ssid = portalServer->arg("ssid");
  String pass = portalServer->arg("pass");
  String city = portalServer->arg("city");
  Config cfg;
  cfg.ssid = ssid;
  cfg.password = pass;
  cfg.city = city;
  ConfigManager::instance().save(cfg);
  String html = "<html><body><h1>Saved</h1><p>Rebooting and connecting...</p></body></html>";
  portalServer->send(200, "text/html", html);
  delay(500);
  ESP.restart();
}

void WifiManagerReal::startProvisionPortal() {
  WiFi.mode(WIFI_AP);
  const char* apName = "DeskBot-Setup";
  WiFi.softAP(apName);
  IPAddress ip = WiFi.softAPIP();
  Serial.printf("Provisioning AP started: %s (%s)\n", apName, ip.toString().c_str());
#if defined(ESP8266)
  if (!portalServer) portalServer = new ESP8266WebServer(80);
#else
  if (!portalServer) portalServer = new WebServer(80);
#endif
  portalServer->on("/", handlePortalRoot);
  portalServer->on("/save", HTTP_POST, handlePortalSave);
  portalServer->begin();
  // Run portal loop in background: simple blocking loop here until saved
  while (true) {
    portalServer->handleClient();
    delay(10);
  }
}

#else
// If USE_WIFI not defined, compile no-op stubs to keep legacy behavior.
void WifiManagerReal::begin(EventBus* bus) { (void)bus; }
void WifiManagerReal::connect() { }
bool WifiManagerReal::isConnected() const { return false; }
void WifiManagerReal::startProvisionPortal() { }
#endif
