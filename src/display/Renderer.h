#pragma once
#include <Arduino.h>

namespace deskbot {

class Renderer {
 public:
  virtual ~Renderer() = default;
  virtual bool begin() = 0;
  virtual void setBrightness(uint8_t brightness) = 0;
  virtual void beginFrame() = 0;
  virtual void clear() = 0;
  virtual void drawText(int16_t x, int16_t y, const String& text, uint8_t size = 1) = 0;
  virtual void drawCenteredText(int16_t y, const String& text, uint8_t size = 1) = 0;
  virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h) = 0;
  virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h) = 0;
  virtual void drawCircle(int16_t x, int16_t y, int16_t r) = 0;
  virtual void fillCircle(int16_t x, int16_t y, int16_t r) = 0;
  virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) = 0;
  virtual void flush() = 0;
  virtual uint16_t width() const = 0;
  virtual uint16_t height() const = 0;
};

}  // namespace deskbot
