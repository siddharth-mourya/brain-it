#include <esp_system.h>
#include "Scheduler.h"

namespace deskbot {

void Scheduler::begin(EventBus& bus) {
  bus_ = &bus;
  uint32_t now = millis();
  lastActivityMs_ = now;
  nextBlinkMs_ = now + randomBetween(6000, 12000);
  nextIdleMs_ = now + randomBetween(90000, 140000);
}

void Scheduler::tick(uint32_t now) {
  if (!bus_) return;
  if (static_cast<int32_t>(now - nextBlinkMs_) >= 0) {
    bus_->emit(EventType::BlinkTrigger);
    nextBlinkMs_ = now + randomBetween(6000, 12000);
  }
  if (static_cast<int32_t>(now - nextIdleMs_) >= 0) {
    bus_->emit(EventType::RandomIdle);
    nextIdleMs_ = now + randomBetween(90000, 140000);
  }
  if (!inactivityEmitted_ && now - lastActivityMs_ > 300000) {
    inactivityEmitted_ = true;
    bus_->emit(EventType::InactivityTimeout);
  }
}

void Scheduler::markActivity(uint32_t now) {
  lastActivityMs_ = now;
  inactivityEmitted_ = false;
}

uint32_t Scheduler::randomBetween(uint32_t minMs, uint32_t maxMs) {
  return minMs + static_cast<uint32_t>(esp_random() % (maxMs - minMs + 1));
}

}  // namespace deskbot
