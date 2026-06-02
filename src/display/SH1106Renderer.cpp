#include "SH1106Renderer.h"
#include <Arduino.h>

#if defined(USE_SH1106)
// If the user enables USE_SH1106, include the real driver.
#include <Adafruit_SH110X.h>
// NOTE: user must configure the display object here and adapt pins.
static Adafruit_SH1106 display = Adafruit_SH1106(128, 64);
#endif

void SH1106Renderer::beginFrame() {
#if defined(USE_SH1106)
  // begin frame if required by driver
#else
  // fallback no-op
#endif
}

void SH1106Renderer::clear() {
#if defined(USE_SH1106)
  display.clearDisplay();
#else
  // fallback to serial visibility
#endif
}

void SH1106Renderer::drawText(int x, int y, const char* text) {
#if defined(USE_SH1106)
  display.setCursor(x, y);
  display.print(text);
#else
  Serial.printf("SH1106Renderer (stub): (%d,%d) %s\n", x, y, text);
#endif
}

void SH1106Renderer::flush() {
#if defined(USE_SH1106)
  display.display();
#else
  // no-op
#endif
}
