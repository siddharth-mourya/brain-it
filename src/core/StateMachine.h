#pragma once
#include "EventBus.h"

class StateMachine {
public:
  enum class State {
    BOOTING,
    PAIRING,
    IDLE,
    EXCITED,
    SLEEPY,
    ERROR,
  };

  void begin(EventBus* bus);
  void update();
  State current() const { return state; }

private:
  EventBus* bus = nullptr;
  State state = State::BOOTING;
};
