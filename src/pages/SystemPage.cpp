#include "SystemPage.h"
#include <WiFi.h>

namespace deskbot {
void SystemPage::render(Renderer& renderer) {
  String state = context_.stateMachine ? context_.stateMachine->stateName() : "UNKNOWN";
  String wifi = context_.wifi ? context_.wifi->ipText() : "offline";
  renderer.drawCenteredText(0, "DeskBot " DESKBOT_FIRMWARE_VERSION);
  renderer.drawText(0, 14, "State: " + state);
  renderer.drawText(0, 24, "WiFi: " + wifi);
  renderer.drawText(0, 34, "Heap: " + String(ESP.getFreeHeap() / 1024) + " KB");
  renderer.drawText(0, 44, "Up: " + String(now_ / 1000) + " sec");
  if (context_.wifi && context_.wifi->portalActive()) renderer.drawText(0, 54, "AP: DeskBot-Setup");
}
}  // namespace deskbot
