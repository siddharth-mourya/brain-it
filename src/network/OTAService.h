#pragma once
#include <Arduino.h>
#include "../core/EventBus.h"

namespace deskbot {

class OTAService {
 public:
  void begin(EventBus& bus, const char* hostname = "deskbot");
  void tick();
  bool active() const { return active_; }

 private:
  EventBus* bus_ = nullptr;
  bool active_ = false;
};

}  // namespace deskbot
