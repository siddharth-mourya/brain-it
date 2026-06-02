#include "OTAService.h"
#include <ArduinoOTA.h>

namespace deskbot {

void OTAService::begin(EventBus& bus, const char* hostname) {
  bus_ = &bus;
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.onStart([this]() {
    active_ = true;
    if (bus_) bus_->emit(EventType::OtaStarted);
  });
  ArduinoOTA.onEnd([this]() {
    active_ = false;
    if (bus_) bus_->emit(EventType::OtaFinished);
  });
  ArduinoOTA.onError([this](ota_error_t error) {
    active_ = false;
    if (bus_) bus_->emit(EventType::ErrorRaised, static_cast<uint32_t>(error), "OTA error");
  });
  ArduinoOTA.begin();
}

void OTAService::tick() { ArduinoOTA.handle(); }

}  // namespace deskbot
