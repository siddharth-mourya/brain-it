#pragma once
#include "../core/EventBus.h"
#include "../config/ConfigManager.h"
#include <functional>

class WifiManagerReal {
public:
  void begin(EventBus* bus);
  void connect();
  bool isConnected() const;
  void startProvisionPortal();
private:
  EventBus* bus = nullptr;
  Config cfg;
  void connectWithConfig();
};
