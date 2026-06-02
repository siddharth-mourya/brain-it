#pragma once
#include <Arduino.h>
#include "EventBus.h"

namespace deskbot {

enum class BotState : uint8_t {
  Booting,
  Pairing,
  Idle,
  Happy,
  Sleepy,
  Excited,
  Annoyed,
  Curious,
  WakeUp,
  FallAsleep,
  Error,
  Updating
};

class StateMachine {
 public:
  void begin(EventBus& bus);
  void update(uint32_t now);
  BotState state() const { return state_; }
  const char* stateName() const;
  uint32_t enteredAt() const { return enteredAt_; }

 private:
  static void onEventStatic(const Event& event, void* context);
  void onEvent(const Event& event);
  void transitionTo(BotState state);
  BotState state_ = BotState::Booting;
  BotState previousBeforeError_ = BotState::Idle;
  uint32_t enteredAt_ = 0;
};

const char* stateName(BotState state);

}  // namespace deskbot
