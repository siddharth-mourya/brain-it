#include "GC9Renderer.h"
#include <Arduino.h>

#if defined(USE_GC9)
// Include actual TFT library here, for example TFT_eSPI. User must enable
// USE_GC9 in build flags and configure TFT_eSPI library in PlatformIO.
#include <TFT_eSPI.h>
static TFT_eSPI tft = TFT_eSPI();
#endif

void GC9Renderer::beginFrame() {
#if defined(USE_GC9)
  // start frame if required
#else
  // stub
#endif
}

void GC9Renderer::clear() {
#if defined(USE_GC9)
  tft.fillScreen(TFT_BLACK);
#else
  // stub
#endif
}

void GC9Renderer::drawText(int x, int y, const char* text) {
#if defined(USE_GC9)
  tft.setCursor(x, y);
  tft.print(text);
#else
  Serial.printf("GC9Renderer (stub): (%d,%d) %s\n", x, y, text);
#endif
}

void GC9Renderer::flush() {
#if defined(USE_GC9)
  // driver flush if needed
#else
  // stub
#endif
}
