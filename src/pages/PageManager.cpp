#include "PageManager.h"
#include <Arduino.h>

void PageManager::registerPage(IPage* p) {
  pages.push_back(p);
}

void PageManager::initPages() {
  for (auto p : pages) p->init();
}

void PageManager::update() {
  if (!pages.empty()) pages[currentIndex]->update();
}

void PageManager::render(Renderer& renderer) {
  if (!pages.empty()) pages[currentIndex]->render(renderer);
}
