#pragma once
#include "EventBus.h"

class Scheduler {
public:
  void begin(EventBus* bus);
  void tick();
private:
  EventBus* bus = nullptr;
  unsigned long lastMillis = 0;
};
