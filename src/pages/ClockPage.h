#pragma once
#include "IPage.h"
#include "../services/TimeService.h"

class ClockPage : public IPage {
public:
  ClockPage(TimeService* ts) : timeService(ts) {}
  void init() override {}
  void update() override {}
  void render(Renderer& renderer) override;
  const char* name() const override { return "Clock"; }
private:
  TimeService* timeService;
};
