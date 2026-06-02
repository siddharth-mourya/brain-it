#include "StateMachine.h"
#include <Arduino.h>

void StateMachine::begin(EventBus* bus) {
  this->bus = bus;
  // react to boot complete
  if (bus) {
    bus->subscribe(EventType::BOOT_COMPLETE, [this]() {
      state = State::IDLE;
      Serial.println("StateMachine: BOOT_COMPLETE -> IDLE");
    });
  }
}

void StateMachine::update() {
  // Minimal placeholder: real logic will use events/mood/timers.
  (void)state;
}
