#pragma once
#include "IPage.h"
#include "PageContext.h"

namespace deskbot {
class SystemPage : public IPage {
 public:
  explicit SystemPage(PageContext& context) : context_(context) {}
  void init() override {}
  void update(uint32_t now) override { now_ = now; }
  void render(Renderer& renderer) override;
  const char* name() const override { return "System"; }
 private:
  PageContext& context_;
  uint32_t now_ = 0;
};
}  // namespace deskbot
