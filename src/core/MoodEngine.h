#pragma once
#include <Arduino.h>
#include "EventBus.h"

namespace deskbot {

struct MoodSnapshot {
  uint8_t happiness;
  uint8_t energy;
  uint8_t excitement;
  uint8_t boredom;
  uint8_t attention;
};

class MoodEngine {
 public:
  void begin(EventBus& bus);
  void update(uint32_t now);
  const MoodSnapshot& snapshot() const { return mood_; }
  const char* dominantMood() const;

 private:
  static void onEventStatic(const Event& event, void* context);
  void onEvent(const Event& event);
  void adjust(uint8_t& value, int delta);
  MoodSnapshot mood_{75, 55, 5, 10, 20};
  uint32_t lastDecayMs_ = 0;
};

}  // namespace deskbot
