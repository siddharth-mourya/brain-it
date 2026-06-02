#include "ThoughtPage.h"
#include <Arduino.h>

void ThoughtPage::init() {
  thoughts = {"Be curious.", "Sip coffee.", "Breathe.", "Make something."};
  idx = 0;
  lastSwitch = millis();
}

void ThoughtPage::update() {
  if (millis() - lastSwitch > 5000) {
    idx = (idx + 1) % thoughts.size();
    lastSwitch = millis();
  }
}

void ThoughtPage::render(Renderer& renderer) {
  renderer.drawText(0, 0, thoughts[idx].c_str());
}
