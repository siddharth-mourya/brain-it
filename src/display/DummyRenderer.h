#pragma once
#include "Renderer.h"
#include <Arduino.h>

class DummyRenderer : public Renderer {
public:
  void beginFrame() override {}
  void clear() override {}
  void drawText(int x, int y, const char* text) override {
    // For the skeleton, print to Serial so behavior is visible.
    Serial.printf("Renderer: drawText (%d,%d): %s\n", x, y, text);
  }
  void flush() override {}
};
