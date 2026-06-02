#include "TouchManager.h"

namespace deskbot {

void TouchManager::begin(EventBus& bus, Scheduler& scheduler, uint8_t pin) {
  bus_ = &bus;
  scheduler_ = &scheduler;
  pin_ = pin;
  pinMode(pin_, INPUT_PULLDOWN);
  lastRaw_ = digitalRead(pin_) == HIGH;
  stable_ = lastRaw_;
  changedAt_ = millis();
}

void TouchManager::poll(uint32_t now) {
  bool raw = digitalRead(pin_) == HIGH;
  if (raw != lastRaw_) {
    lastRaw_ = raw;
    changedAt_ = now;
  }
  if (now - changedAt_ < 35 || raw == stable_) {
    if (stable_ && !longEmitted_ && now - pressedAt_ >= 4000) {
      longEmitted_ = true;
      pendingTaps_ = 0;
      if (scheduler_) scheduler_->markActivity(now);
      if (bus_) bus_->emit(EventType::TouchLong);
    }
    if (!stable_ && pendingTaps_ == 1 && now - lastReleaseAt_ > 350) {
      pendingTaps_ = 0;
      if (scheduler_) scheduler_->markActivity(now);
      if (bus_) bus_->emit(EventType::TouchSingle);
    }
    return;
  }

  stable_ = raw;
  if (stable_) {
    pressedAt_ = now;
    longEmitted_ = false;
  } else if (!longEmitted_) {
    if (pendingTaps_ == 1 && now - lastReleaseAt_ <= 350) {
      pendingTaps_ = 0;
      if (scheduler_) scheduler_->markActivity(now);
      if (bus_) bus_->emit(EventType::TouchDouble);
    } else {
      pendingTaps_ = 1;
      lastReleaseAt_ = now;
    }
  }
}

}  // namespace deskbot
