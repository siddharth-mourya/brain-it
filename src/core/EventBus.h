#pragma once
#include <Arduino.h>

namespace deskbot {

enum class EventType : uint8_t {
  BootComplete,
  TouchSingle,
  TouchDouble,
  TouchLong,
  PresenceDetected,
  PresenceLost,
  BlinkTrigger,
  RandomIdle,
  InactivityTimeout,
  WeatherUpdated,
  WeatherFailed,
  WifiConnected,
  WifiDisconnected,
  PairingStarted,
  ConfigUpdated,
  OtaStarted,
  OtaFinished,
  ErrorRaised,
  ErrorCleared
};

struct Event {
  EventType type;
  uint32_t value;
  const char* message;
};

using EventHandler = void (*)(const Event& event, void* context);

class EventBus {
 public:
  bool emit(EventType type, uint32_t value = 0, const char* message = nullptr);
  bool subscribe(EventHandler handler, void* context = nullptr);
  void dispatch();
  uint8_t queued() const { return count_; }
  uint32_t dropped() const { return dropped_; }

 private:
  static constexpr uint8_t kQueueSize = 24;
  static constexpr uint8_t kMaxSubscribers = 12;
  Event queue_[kQueueSize]{};
  EventHandler handlers_[kMaxSubscribers]{};
  void* contexts_[kMaxSubscribers]{};
  uint8_t head_ = 0;
  uint8_t tail_ = 0;
  uint8_t count_ = 0;
  uint8_t subscribers_ = 0;
  uint32_t dropped_ = 0;
};

const char* eventName(EventType type);

}  // namespace deskbot
