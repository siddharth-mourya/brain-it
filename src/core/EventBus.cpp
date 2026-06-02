#include "EventBus.h"

namespace deskbot {

bool EventBus::emit(EventType type, uint32_t value, const char* message) {
  if (count_ >= kQueueSize) {
    dropped_++;
    return false;
  }
  queue_[tail_] = {type, value, message};
  tail_ = (tail_ + 1) % kQueueSize;
  count_++;
  return true;
}

bool EventBus::subscribe(EventHandler handler, void* context) {
  if (!handler || subscribers_ >= kMaxSubscribers) return false;
  handlers_[subscribers_] = handler;
  contexts_[subscribers_] = context;
  subscribers_++;
  return true;
}

void EventBus::dispatch() {
  while (count_ > 0) {
    Event event = queue_[head_];
    head_ = (head_ + 1) % kQueueSize;
    count_--;
    for (uint8_t i = 0; i < subscribers_; ++i) {
      handlers_[i](event, contexts_[i]);
    }
  }
}

const char* eventName(EventType type) {
  switch (type) {
    case EventType::BootComplete: return "BOOT_COMPLETE";
    case EventType::TouchSingle: return "TOUCH_SINGLE";
    case EventType::TouchDouble: return "TOUCH_DOUBLE";
    case EventType::TouchLong: return "TOUCH_LONG";
    case EventType::PresenceDetected: return "PRESENCE_DETECTED";
    case EventType::PresenceLost: return "PRESENCE_LOST";
    case EventType::BlinkTrigger: return "BLINK_TRIGGER";
    case EventType::RandomIdle: return "RANDOM_IDLE";
    case EventType::InactivityTimeout: return "INACTIVITY_TIMEOUT";
    case EventType::WeatherUpdated: return "WEATHER_UPDATED";
    case EventType::WeatherFailed: return "WEATHER_FAILED";
    case EventType::WifiConnected: return "WIFI_CONNECTED";
    case EventType::WifiDisconnected: return "WIFI_DISCONNECTED";
    case EventType::PairingStarted: return "PAIRING_STARTED";
    case EventType::ConfigUpdated: return "CONFIG_UPDATED";
    case EventType::OtaStarted: return "OTA_STARTED";
    case EventType::OtaFinished: return "OTA_FINISHED";
    case EventType::ErrorRaised: return "ERROR_RAISED";
    case EventType::ErrorCleared: return "ERROR_CLEARED";
  }
  return "UNKNOWN";
}

}  // namespace deskbot
