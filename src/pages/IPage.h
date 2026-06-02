#pragma once
#include <Arduino.h>
#include "../display/Renderer.h"

namespace deskbot {

class IPage {
 public:
  virtual ~IPage() = default;
  virtual void init() = 0;
  virtual void update(uint32_t now) = 0;
  virtual void render(Renderer& renderer) = 0;
  virtual const char* name() const = 0;
};

}  // namespace deskbot
