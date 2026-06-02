#pragma once
#include <Arduino.h>
#include "EventBus.h"

namespace deskbot {

class Scheduler {
 public:
  void begin(EventBus& bus);
  void tick(uint32_t now);
  void markActivity(uint32_t now);

 private:
  EventBus* bus_ = nullptr;
  uint32_t nextBlinkMs_ = 0;
  uint32_t nextIdleMs_ = 0;
  uint32_t lastActivityMs_ = 0;
  bool inactivityEmitted_ = false;
  uint32_t randomBetween(uint32_t minMs, uint32_t maxMs);
};

}  // namespace deskbot
