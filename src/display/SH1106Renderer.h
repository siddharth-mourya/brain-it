#pragma once
#include "Renderer.h"
#include <Adafruit_SH110X.h>
#include <Wire.h>

namespace deskbot {

class SH1106Renderer : public Renderer {
 public:
  bool begin() override;
  void setBrightness(uint8_t brightness) override;
  void beginFrame() override;
  void clear() override;
  void drawText(int16_t x, int16_t y, const String& text, uint8_t size = 1) override;
  void drawCenteredText(int16_t y, const String& text, uint8_t size = 1) override;
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h) override;
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h) override;
  void drawCircle(int16_t x, int16_t y, int16_t r) override;
  void fillCircle(int16_t x, int16_t y, int16_t r) override;
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) override;
  void flush() override;
  uint16_t width() const override { return DESKBOT_DISPLAY_WIDTH; }
  uint16_t height() const override { return DESKBOT_DISPLAY_HEIGHT; }

 private:
  Adafruit_SH1106G display_{DESKBOT_DISPLAY_WIDTH, DESKBOT_DISPLAY_HEIGHT, &Wire, -1};
};

}  // namespace deskbot
