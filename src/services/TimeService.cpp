#include "TimeService.h"
#if defined(USE_TIME)
#include <Arduino.h>
#include <time.h>

void TimeService::begin(EventBus* bus) {
  (void)bus;
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0;
  const int daylightOffset_sec = 0;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  int retries = 0;
  while (retries++ < 20) {
    time_t t = time(nullptr);
    if (t > 100000) {
      synced = true;
      break;
    }
    delay(500);
  }
}

time_t TimeService::now() const {
  return time(nullptr);
}

#else
void TimeService::begin(EventBus* bus) { (void)bus; }
time_t TimeService::now() const { return 0; }
#endif
