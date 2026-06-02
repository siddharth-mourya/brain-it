#pragma once

class Renderer {
public:
  virtual ~Renderer() = default;
  virtual void beginFrame() = 0;
  virtual void clear() = 0;
  virtual void drawText(int x, int y, const char* text) = 0;
  virtual void flush() = 0;
};
