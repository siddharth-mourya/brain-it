#pragma once
#include "IPage.h"
#include "PageContext.h"

namespace deskbot {
class WeatherPage : public IPage {
 public:
  explicit WeatherPage(PageContext& context) : context_(context) {}
  void init() override {}
  void update(uint32_t now) override { (void)now; }
  void render(Renderer& renderer) override;
  const char* name() const override { return "Weather"; }
 private:
  PageContext& context_;
};
}  // namespace deskbot
