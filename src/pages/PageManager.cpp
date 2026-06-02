#include "PageManager.h"

namespace deskbot {

void PageManager::begin(EventBus& bus) { bus.subscribe(onEventStatic, this); }

bool PageManager::registerPage(IPage* page) {
  if (!page || count_ >= kMaxPages) return false;
  pages_[count_++] = page;
  page->init();
  return true;
}

void PageManager::update(uint32_t now) {
  if (count_ > 0) pages_[current_]->update(now);
}

void PageManager::render(Renderer& renderer) {
  if (count_ == 0) {
    renderer.drawCenteredText(24, "No pages");
    return;
  }
  pages_[current_]->render(renderer);
  renderer.drawText(0, 56, String(current_ + 1) + "/" + String(count_) + " " + pages_[current_]->name());
}

void PageManager::next() { if (count_ > 0) current_ = (current_ + 1) % count_; }
void PageManager::previous() { if (count_ > 0) current_ = (current_ + count_ - 1) % count_; }
void PageManager::showSystemPage() { if (count_ > 0) current_ = count_ - 1; }
const char* PageManager::currentName() const { return count_ > 0 ? pages_[current_]->name() : "none"; }

void PageManager::onEventStatic(const Event& event, void* context) { static_cast<PageManager*>(context)->onEvent(event); }

void PageManager::onEvent(const Event& event) {
  switch (event.type) {
    case EventType::TouchSingle: next(); break;
    case EventType::TouchDouble: previous(); break;
    case EventType::TouchLong:
    case EventType::PairingStarted:
    case EventType::WifiDisconnected:
    case EventType::ErrorRaised:
    case EventType::OtaStarted:
      showSystemPage();
      break;
    default:
      break;
  }
}

}  // namespace deskbot
