#pragma once
#include "../core/EventBus.h"
#include "WifiManagerReal.h"

// Wrapper to select between stub and real implementation via build flags.
class WifiManager {
public:
  void begin(EventBus* bus);
  void connect();
  bool isConnected() const;
private:
  WifiManagerReal real;
};
