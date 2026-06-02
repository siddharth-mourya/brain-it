#include <esp_system.h>
#include "ThoughtPage.h"

namespace deskbot {
namespace {
const char* kThoughts[] = {
    "Small steps count.",
    "Hydrate, human.",
    "Focus beats busy.",
    "Blink. Breathe. Build.",
    "Your desk has vibes.",
};
constexpr uint8_t kThoughtCount = sizeof(kThoughts) / sizeof(kThoughts[0]);
}

void ThoughtPage::init() { index_ = esp_random() % kThoughtCount; }

void ThoughtPage::update(uint32_t now) {
  if (nextChangeMs_ == 0) nextChangeMs_ = now + 60000;
  if (now > nextChangeMs_) {
    index_ = (index_ + 1) % kThoughtCount;
    nextChangeMs_ = now + 60000;
  }
}

void ThoughtPage::render(Renderer& renderer) {
  (void)context_;
  renderer.drawCenteredText(8, "Thought of day");
  renderer.drawRect(6, 22, 116, 28);
  renderer.drawCenteredText(32, kThoughts[index_]);
}
}  // namespace deskbot
