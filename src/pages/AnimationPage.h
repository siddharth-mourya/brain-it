#pragma once
#include "IPage.h"
#include "PageContext.h"

namespace deskbot {
class AnimationPage : public IPage {
 public:
  explicit AnimationPage(PageContext& context) : context_(context) {}
  void init() override {}
  void update(uint32_t now) override { frame_ = (now / 120) % 24; }
  void render(Renderer& renderer) override;
  const char* name() const override { return "Anim"; }
 private:
  PageContext& context_;
  uint8_t frame_ = 0;
};
}  // namespace deskbot
