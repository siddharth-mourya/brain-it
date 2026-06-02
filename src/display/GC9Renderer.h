#pragma once
#include "Renderer.h"

// Optional GC9 renderer wrapper (color TFT). Requires the user's preferred
// TFT driver library (e.g., TFT_eSPI). If not enabled, this acts as stub.

class GC9Renderer : public Renderer {
public:
  void beginFrame() override;
  void clear() override;
  void drawText(int x, int y, const char* text) override;
  void flush() override;
private:
};
