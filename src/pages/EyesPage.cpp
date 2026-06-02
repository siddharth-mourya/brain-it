#include "EyesPage.h"

namespace deskbot {

void EyesPage::init() {}

void EyesPage::update(uint32_t now) {
  if (!blinking_ && (now % 7300UL) < 80) {
    blinking_ = true;
    blinkUntilMs_ = now + 160;
  }
  if (blinking_ && now > blinkUntilMs_) blinking_ = false;
}

void EyesPage::render(Renderer& renderer) {
  BotState state = context_.stateMachine ? context_.stateMachine->state() : BotState::Idle;
  int eyeY = (state == BotState::Sleepy) ? 34 : 30;
  if (blinking_ || state == BotState::Sleepy) {
    renderer.drawLine(28, eyeY, 50, eyeY);
    renderer.drawLine(78, eyeY, 100, eyeY);
  } else if (state == BotState::Excited || state == BotState::WakeUp) {
    renderer.fillCircle(39, eyeY, 12);
    renderer.fillCircle(89, eyeY, 12);
    renderer.fillCircle(43, eyeY - 4, 3);
    renderer.fillCircle(93, eyeY - 4, 3);
  } else if (state == BotState::Error) {
    renderer.drawLine(28, 22, 50, 38);
    renderer.drawLine(50, 22, 28, 38);
    renderer.drawLine(78, 22, 100, 38);
    renderer.drawLine(100, 22, 78, 38);
  } else {
    renderer.drawCircle(39, eyeY, 11);
    renderer.drawCircle(89, eyeY, 11);
    renderer.fillCircle(39, eyeY, 5);
    renderer.fillCircle(89, eyeY, 5);
  }
  const char* mood = context_.moodEngine ? context_.moodEngine->dominantMood() : "calm";
  renderer.drawCenteredText(46, String("mood: ") + mood);
}

}  // namespace deskbot
