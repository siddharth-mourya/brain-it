#include "WifiManager.h"

void WifiManager::begin(EventBus* bus) {
  real.begin(bus);
}

void WifiManager::connect() {
  real.connect();
}

bool WifiManager::isConnected() const {
  return real.isConnected();
}
