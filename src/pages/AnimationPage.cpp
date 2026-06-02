#include "AnimationPage.h"

namespace deskbot {
void AnimationPage::render(Renderer& renderer) {
  (void)context_;
  int16_t x = 16 + frame_ * 4;
  if (x > 112) x = 112 - (x - 112);
  renderer.drawCenteredText(4, "Animation viewer");
  renderer.drawRect(8, 18, 112, 34);
  renderer.fillCircle(x, 35, 6);
  renderer.drawLine(18, 47, 110, 47);
}
}  // namespace deskbot
