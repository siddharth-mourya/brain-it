#include "ClockPage.h"

namespace deskbot {
void ClockPage::render(Renderer& renderer) {
  String time = context_.time ? context_.time->timeText() : "--:--";
  String date = context_.time ? context_.time->dateText() : "No time";
  renderer.drawCenteredText(14, time, 2);
  renderer.drawCenteredText(38, date);
}
}  // namespace deskbot
