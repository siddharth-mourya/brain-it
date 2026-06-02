#include "RgbEngine.h"
#include <math.h>

namespace deskbot {

void RgbEngine::begin(uint8_t pin, uint16_t count) {
  pin_ = pin;
  count_ = count;
  strip_.updateLength(count_);
  strip_.setPin(pin_);
  strip_.begin();
  strip_.setBrightness(48);
  strip_.show();
}

void RgbEngine::update(uint32_t now, BotState state, const MoodSnapshot& mood) {
  if (now - lastUpdateMs_ < 60) return;
  lastUpdateMs_ = now;
  uint8_t breath = static_cast<uint8_t>((sin(now / 700.0F) + 1.0F) * 35.0F + 15.0F);
  uint32_t color;
  switch (state) {
    case BotState::Error:
      color = strip_.Color(90, 0, 0);
      break;
    case BotState::Updating:
      color = strip_.Color(0, 0, 90);
      break;
    case BotState::Pairing:
      color = strip_.Color(65, 35, 0);
      break;
    case BotState::Sleepy:
      color = strip_.Color(0, 0, breath);
      break;
    case BotState::Excited:
    case BotState::WakeUp:
      color = strip_.Color(breath, breath / 2, 0);
      break;
    default:
      color = strip_.Color(0, mood.happiness > 80 ? 80 : mood.happiness, breath / 2);
      break;
  }
  for (uint16_t i = 0; i < count_; ++i) strip_.setPixelColor(i, color);
  strip_.show();
}

}  // namespace deskbot
