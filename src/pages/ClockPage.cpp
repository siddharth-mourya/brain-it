#include "ClockPage.h"
#include <Arduino.h>
#include <time.h>

void ClockPage::render(Renderer& renderer) {
  time_t t = 0;
#if defined(USE_TIME)
  if (timeService) t = timeService->now();
#endif
  if (t == 0) renderer.drawText(0, 0, "Clock: --:--:--");
  else {
    struct tm* tm = localtime(&t);
    char buf[32];
    snprintf(buf, sizeof(buf), "Time: %02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
    renderer.drawText(0, 0, buf);
  }
}
