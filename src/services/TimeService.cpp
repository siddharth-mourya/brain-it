#include "TimeService.h"
#include <WiFi.h>

namespace deskbot {

void TimeService::begin() { configTime(19800, 0, "pool.ntp.org", "time.nist.gov"); }
void TimeService::tick() {}

bool TimeService::hasTime() const {
  time_t now = time(nullptr);
  return now > 1700000000;
}

String TimeService::timeText() const {
  if (!hasTime()) return "--:--";
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  char buf[8];
  strftime(buf, sizeof(buf), "%H:%M", &timeinfo);
  return String(buf);
}

String TimeService::dateText() const {
  if (!hasTime()) return "Syncing time";
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  char buf[18];
  strftime(buf, sizeof(buf), "%d %b %Y", &timeinfo);
  return String(buf);
}

}  // namespace deskbot
