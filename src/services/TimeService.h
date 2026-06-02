#pragma once
#include <Arduino.h>
#include <time.h>

namespace deskbot {

class TimeService {
 public:
  void begin();
  void tick();
  bool hasTime() const;
  String timeText() const;
  String dateText() const;
};

}  // namespace deskbot
