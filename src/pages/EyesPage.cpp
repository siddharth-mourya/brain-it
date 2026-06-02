#include "EyesPage.h"
#include <Arduino.h>

void EyesPage::init() {
  lastBlink = millis();
  blinkState = false;
}

void EyesPage::update() {
  unsigned long now = millis();
  if (now - lastBlink > 2000) {
    blinkState = !blinkState;
    lastBlink = now;
  }
}

void EyesPage::render(Renderer& renderer) {
  renderer.clear();
  if (blinkState) renderer.drawText(0, 0, "Eyes: <- >");
  else renderer.drawText(0, 0, "Eyes: -  -");
}
