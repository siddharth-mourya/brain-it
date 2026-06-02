#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../core/StateMachine.h"
#include "../core/MoodEngine.h"

namespace deskbot {

class RgbEngine {
 public:
  void begin(uint8_t pin = DESKBOT_RGB_PIN, uint16_t count = DESKBOT_RGB_COUNT);
  void update(uint32_t now, BotState state, const MoodSnapshot& mood);

 private:
  Adafruit_NeoPixel strip_{DESKBOT_RGB_COUNT, DESKBOT_RGB_PIN, NEO_GRB + NEO_KHZ800};
  uint8_t pin_ = DESKBOT_RGB_PIN;
  uint16_t count_ = DESKBOT_RGB_COUNT;
  uint32_t lastUpdateMs_ = 0;
};

}  // namespace deskbot
