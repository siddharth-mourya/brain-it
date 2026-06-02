#include "StateMachine.h"

namespace deskbot {

void StateMachine::begin(EventBus& bus) { bus.subscribe(onEventStatic, this); }

void StateMachine::update(uint32_t now) {
  if ((state_ == BotState::Excited || state_ == BotState::Happy || state_ == BotState::WakeUp) && now - enteredAt_ > 2500) {
    transitionTo(BotState::Idle);
  }
  if (state_ == BotState::Annoyed && now - enteredAt_ > 3500) transitionTo(BotState::Idle);
}

const char* StateMachine::stateName() const { return deskbot::stateName(state_); }

void StateMachine::onEventStatic(const Event& event, void* context) {
  static_cast<StateMachine*>(context)->onEvent(event);
}

void StateMachine::onEvent(const Event& event) {
  if (state_ == BotState::Updating && event.type != EventType::OtaFinished) return;

  switch (event.type) {
    case EventType::BootComplete:
      transitionTo(BotState::Idle);
      break;
    case EventType::PairingStarted:
      transitionTo(BotState::Pairing);
      break;
    case EventType::TouchSingle:
    case EventType::TouchDouble:
      transitionTo(BotState::Excited);
      break;
    case EventType::TouchLong:
      transitionTo(BotState::Curious);
      break;
    case EventType::PresenceDetected:
      transitionTo(BotState::WakeUp);
      break;
    case EventType::InactivityTimeout:
      transitionTo(BotState::Sleepy);
      break;
    case EventType::WifiConnected:
    case EventType::ErrorCleared:
      if (state_ == BotState::Error || state_ == BotState::Pairing) transitionTo(BotState::Idle);
      break;
    case EventType::WifiDisconnected:
    case EventType::WeatherFailed:
    case EventType::ErrorRaised:
      previousBeforeError_ = state_;
      transitionTo(BotState::Error);
      break;
    case EventType::OtaStarted:
      transitionTo(BotState::Updating);
      break;
    case EventType::OtaFinished:
      transitionTo(previousBeforeError_);
      break;
    default:
      break;
  }
}

void StateMachine::transitionTo(BotState state) {
  state_ = state;
  enteredAt_ = millis();
}

const char* stateName(BotState state) {
  switch (state) {
    case BotState::Booting: return "BOOTING";
    case BotState::Pairing: return "PAIRING";
    case BotState::Idle: return "IDLE";
    case BotState::Happy: return "HAPPY";
    case BotState::Sleepy: return "SLEEPY";
    case BotState::Excited: return "EXCITED";
    case BotState::Annoyed: return "ANNOYED";
    case BotState::Curious: return "CURIOUS";
    case BotState::WakeUp: return "WAKE_UP";
    case BotState::FallAsleep: return "FALL_ASLEEP";
    case BotState::Error: return "ERROR";
    case BotState::Updating: return "UPDATING";
  }
  return "UNKNOWN";
}

}  // namespace deskbot
