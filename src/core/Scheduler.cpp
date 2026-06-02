#include "Scheduler.h"
#include <Arduino.h>

void Scheduler::begin(EventBus* bus) {
  this->bus = bus;
  lastMillis = millis();
}

void Scheduler::tick() {
  unsigned long now = millis();
  if (now - lastMillis >= 1000) {
    lastMillis = now;
    // fire a simple 1s timer event if needed
  }
}
