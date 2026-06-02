#include "SystemPage.h"
#include <Arduino.h>

void SystemPage::render(Renderer& renderer) {
  char buf[128];
  snprintf(buf, sizeof(buf), "System: State=%d", (int)stateMachine->current());
  renderer.drawText(0,0,buf);
}
