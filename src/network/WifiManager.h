#pragma once
#include <Arduino.h>
#include <WebServer.h>
#include "../config/ConfigManager.h"
#include "../core/EventBus.h"

namespace deskbot {

class WifiManager {
 public:
  void begin(EventBus& bus, ConfigManager& config);
  void tick(uint32_t now);
  bool connected() const;
  bool portalActive() const { return portalActive_; }
  String ipText() const;

 private:
  void startPortal();
  void connectStation();
  void handleRoot();
  void handleSave();
  String htmlPage() const;
  EventBus* bus_ = nullptr;
  ConfigManager* config_ = nullptr;
  WebServer server_{80};
  bool portalActive_ = false;
  bool lastConnected_ = false;
  uint32_t lastReconnectMs_ = 0;
};

}  // namespace deskbot
