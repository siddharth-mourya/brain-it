#pragma once
#include "IPage.h"
#include <vector>

class PageManager {
public:
  void registerPage(IPage* p);
  void initPages();
  void update();
  void render(Renderer& renderer);
private:
  std::vector<IPage*> pages;
  size_t currentIndex = 0;
};
