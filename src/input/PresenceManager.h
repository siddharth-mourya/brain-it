#pragma once
#include <Arduino.h>
#include "../core/EventBus.h"
#include "../core/Scheduler.h"

namespace deskbot {

class PresenceManager {
 public:
  void begin(EventBus& bus, Scheduler& scheduler, uint8_t pin = DESKBOT_PRESENCE_PIN);
  void poll(uint32_t now);
  bool present() const { return present_; }

 private:
  EventBus* bus_ = nullptr;
  Scheduler* scheduler_ = nullptr;
  uint8_t pin_ = DESKBOT_PRESENCE_PIN;
  bool present_ = false;
  bool lastRaw_ = false;
  uint32_t changedAt_ = 0;
};

}  // namespace deskbot
