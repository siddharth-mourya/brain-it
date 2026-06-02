#pragma once
#include "../display/Renderer.h"

class IPage {
public:
  virtual ~IPage() = default;
  virtual void init() = 0;
  virtual void update() = 0;
  virtual void render(Renderer& renderer) = 0;
  virtual const char* name() const = 0;
};
