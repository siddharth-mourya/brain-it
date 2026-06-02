#include "MoodEngine.h"

namespace deskbot {

void MoodEngine::begin(EventBus& bus) { bus.subscribe(onEventStatic, this); }

void MoodEngine::update(uint32_t now) {
  if (now - lastDecayMs_ < 5000) return;
  lastDecayMs_ = now;
  adjust(mood_.excitement, -1);
  adjust(mood_.attention, -1);
  adjust(mood_.boredom, 1);
  if (mood_.energy > 25) adjust(mood_.energy, -1);
}

const char* MoodEngine::dominantMood() const {
  if (mood_.excitement > 55) return "excited";
  if (mood_.energy < 25) return "sleepy";
  if (mood_.boredom > 70) return "bored";
  if (mood_.happiness > 70) return "happy";
  return "calm";
}

void MoodEngine::onEventStatic(const Event& event, void* context) {
  static_cast<MoodEngine*>(context)->onEvent(event);
}

void MoodEngine::onEvent(const Event& event) {
  switch (event.type) {
    case EventType::TouchSingle:
    case EventType::TouchDouble:
      adjust(mood_.excitement, 24);
      adjust(mood_.attention, 20);
      adjust(mood_.boredom, -18);
      break;
    case EventType::TouchLong:
      adjust(mood_.attention, 25);
      break;
    case EventType::PresenceDetected:
      adjust(mood_.attention, 28);
      adjust(mood_.energy, 12);
      break;
    case EventType::WeatherUpdated:
      adjust(mood_.happiness, 3);
      break;
    case EventType::WeatherFailed:
    case EventType::WifiDisconnected:
    case EventType::ErrorRaised:
      adjust(mood_.happiness, -8);
      break;
    case EventType::InactivityTimeout:
      adjust(mood_.energy, -20);
      adjust(mood_.attention, -15);
      break;
    default:
      break;
  }
}

void MoodEngine::adjust(uint8_t& value, int delta) {
  int next = static_cast<int>(value) + delta;
  value = static_cast<uint8_t>(constrain(next, 0, 100));
}

}  // namespace deskbot
