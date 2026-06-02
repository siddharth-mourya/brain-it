#include "PresenceManager.h"

namespace deskbot {

void PresenceManager::begin(EventBus& bus, Scheduler& scheduler, uint8_t pin) {
  bus_ = &bus;
  scheduler_ = &scheduler;
  pin_ = pin;
  pinMode(pin_, INPUT_PULLDOWN);
  lastRaw_ = digitalRead(pin_) == HIGH;
  present_ = lastRaw_;
  changedAt_ = millis();
}

void PresenceManager::poll(uint32_t now) {
  bool raw = digitalRead(pin_) == HIGH;
  if (raw != lastRaw_) {
    lastRaw_ = raw;
    changedAt_ = now;
  }
  if (now - changedAt_ < 100 || raw == present_) return;
  present_ = raw;
  if (present_) {
    if (scheduler_) scheduler_->markActivity(now);
    if (bus_) bus_->emit(EventType::PresenceDetected);
  } else if (bus_) {
    bus_->emit(EventType::PresenceLost);
  }
}

}  // namespace deskbot
