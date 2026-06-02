#pragma once
#include <ctime>
#include "../core/EventBus.h"

class TimeService {
public:
  void begin(EventBus* bus);
  time_t now() const;
private:
  bool synced = false;
};
