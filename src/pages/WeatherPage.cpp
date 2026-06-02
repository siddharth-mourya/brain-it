#include "WeatherPage.h"

namespace deskbot {
void WeatherPage::render(Renderer& renderer) {
  const AppConfig& cfg = context_.config->config();
  const WeatherSnapshot& weather = context_.weather->snapshot();
  renderer.drawCenteredText(4, cfg.city);
  if (!weather.valid) {
    renderer.drawCenteredText(26, "Weather loading");
    renderer.drawCenteredText(40, "or offline");
    return;
  }
  renderer.drawCenteredText(20, String(weather.temperatureC, 1) + " C", 2);
  renderer.drawCenteredText(42, weather.summary + "  Wind " + String(weather.windKph, 0));
}
}  // namespace deskbot
