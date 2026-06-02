#include "SH1106Renderer.h"

namespace deskbot {

bool SH1106Renderer::begin() {
  Wire.begin(DESKBOT_I2C_SDA, DESKBOT_I2C_SCL);
  if (!display_.begin(0x3C, true)) return false;
  display_.setTextColor(SH110X_WHITE);
  display_.setTextWrap(false);
  clear();
  flush();
  return true;
}

void SH1106Renderer::setBrightness(uint8_t brightness) { display_.setContrast(brightness); }
void SH1106Renderer::beginFrame() { clear(); }
void SH1106Renderer::clear() { display_.clearDisplay(); }
void SH1106Renderer::drawText(int16_t x, int16_t y, const String& text, uint8_t size) {
  display_.setTextSize(size);
  display_.setCursor(x, y);
  display_.print(text);
}
void SH1106Renderer::drawCenteredText(int16_t y, const String& text, uint8_t size) {
  int16_t x1, y1;
  uint16_t w, h;
  display_.setTextSize(size);
  display_.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  int16_t centered = static_cast<int16_t>((width() - w) / 2);
  int16_t x = centered < 0 ? 0 : centered;
  drawText(x, y, text, size);
}
void SH1106Renderer::drawRect(int16_t x, int16_t y, int16_t w, int16_t h) { display_.drawRect(x, y, w, h, SH110X_WHITE); }
void SH1106Renderer::fillRect(int16_t x, int16_t y, int16_t w, int16_t h) { display_.fillRect(x, y, w, h, SH110X_WHITE); }
void SH1106Renderer::drawCircle(int16_t x, int16_t y, int16_t r) { display_.drawCircle(x, y, r, SH110X_WHITE); }
void SH1106Renderer::fillCircle(int16_t x, int16_t y, int16_t r) { display_.fillCircle(x, y, r, SH110X_WHITE); }
void SH1106Renderer::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) { display_.drawLine(x0, y0, x1, y1, SH110X_WHITE); }
void SH1106Renderer::flush() { display_.display(); }

}  // namespace deskbot
