#include "OtaManager.h"
#if defined(USE_OTA)
#include <ArduinoOTA.h>
#include <Arduino.h>

void OtaManager::begin() {
  ArduinoOTA.onStart([]() { Serial.println("OTA: start"); });
  ArduinoOTA.onEnd([]() { Serial.println("OTA: end"); });
  ArduinoOTA.onError([](ota_error_t err) { Serial.printf("OTA error: %u\n", err); });
  ArduinoOTA.begin();
  Serial.println("OTA: ready");
}

#else
void OtaManager::begin() { }
#endif
