#pragma once
#include "IPage.h"
#include "../core/StateMachine.h"

class SystemPage : public IPage {
public:
  SystemPage(StateMachine* sm) : stateMachine(sm) {}
  void init() override {}
  void update() override {}
  void render(Renderer& renderer) override;
  const char* name() const override { return "System"; }
private:
  StateMachine* stateMachine;
};
