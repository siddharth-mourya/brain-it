#pragma once
#include <Arduino.h>
#include "../core/EventBus.h"
#include "../core/Scheduler.h"

namespace deskbot {

class TouchManager {
 public:
  void begin(EventBus& bus, Scheduler& scheduler, uint8_t pin = DESKBOT_TOUCH_PIN);
  void poll(uint32_t now);

 private:
  EventBus* bus_ = nullptr;
  Scheduler* scheduler_ = nullptr;
  uint8_t pin_ = DESKBOT_TOUCH_PIN;
  bool lastRaw_ = false;
  bool stable_ = false;
  bool longEmitted_ = false;
  uint32_t changedAt_ = 0;
  uint32_t pressedAt_ = 0;
  uint32_t lastReleaseAt_ = 0;
  uint8_t pendingTaps_ = 0;
};

}  // namespace deskbot
