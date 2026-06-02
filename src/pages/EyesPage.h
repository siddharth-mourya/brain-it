#pragma once
#include "IPage.h"

class EyesPage : public IPage {
public:
  void init() override;
  void update() override;
  void render(Renderer& renderer) override;
  const char* name() const override { return "Eyes"; }
private:
  unsigned long lastBlink = 0;
  bool blinkState = false;
};
