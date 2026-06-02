#pragma once
#include "IPage.h"
#include "PageContext.h"

namespace deskbot {
class ThoughtPage : public IPage {
 public:
  explicit ThoughtPage(PageContext& context) : context_(context) {}
  void init() override;
  void update(uint32_t now) override;
  void render(Renderer& renderer) override;
  const char* name() const override { return "Thought"; }
 private:
  PageContext& context_;
  uint8_t index_ = 0;
  uint32_t nextChangeMs_ = 0;
};
}  // namespace deskbot
