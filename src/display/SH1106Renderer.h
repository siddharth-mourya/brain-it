#pragma once
#include "Renderer.h"

// Optional SH1106 renderer wrapper.
// To enable full SH1106 support define USE_SH1106 and add the
// Adafruit_SH110X library to PlatformIO `lib_deps`.

class SH1106Renderer : public Renderer {
public:
  void beginFrame() override;
  void clear() override;
  void drawText(int x, int y, const char* text) override;
  void flush() override;
private:
};
