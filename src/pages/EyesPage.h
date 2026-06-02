#pragma once
#include "IPage.h"
#include "PageContext.h"

namespace deskbot {

class EyesPage : public IPage {
 public:
  explicit EyesPage(PageContext& context) : context_(context) {}
  void init() override;
  void update(uint32_t now) override;
  void render(Renderer& renderer) override;
  const char* name() const override { return "Eyes"; }

 private:
  PageContext& context_;
  bool blinking_ = false;
  uint32_t blinkUntilMs_ = 0;
};

}  // namespace deskbot
