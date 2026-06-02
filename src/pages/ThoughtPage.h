#pragma once
#include <Arduino.h>
#include "IPage.h"
#include <vector>

class ThoughtPage : public IPage {
public:
  void init() override;
  void update() override;
  void render(Renderer& renderer) override;
  const char* name() const override { return "Thought"; }
private:
  std::vector<String> thoughts;
  size_t idx = 0;
  unsigned long lastSwitch = 0;
};
