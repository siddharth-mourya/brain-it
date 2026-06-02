#pragma once
#include <Arduino.h>
#include "IPage.h"
#include "../core/EventBus.h"

namespace deskbot {

class PageManager {
 public:
  void begin(EventBus& bus);
  bool registerPage(IPage* page);
  void update(uint32_t now);
  void render(Renderer& renderer);
  void next();
  void previous();
  void showSystemPage();
  const char* currentName() const;
  uint8_t currentIndex() const { return current_; }
  uint8_t count() const { return count_; }

 private:
  static void onEventStatic(const Event& event, void* context);
  void onEvent(const Event& event);
  static constexpr uint8_t kMaxPages = 8;
  IPage* pages_[kMaxPages]{};
  uint8_t count_ = 0;
  uint8_t current_ = 0;
};

}  // namespace deskbot
