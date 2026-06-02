#pragma once
#include "../config/ConfigManager.h"
#include "../core/EventBus.h"
#include "../core/MoodEngine.h"
#include "../core/StateMachine.h"
#include "../network/WifiManager.h"
#include "../services/TimeService.h"
#include "../services/WeatherService.h"

namespace deskbot {

struct PageContext {
  EventBus* bus = nullptr;
  StateMachine* stateMachine = nullptr;
  MoodEngine* moodEngine = nullptr;
  ConfigManager* config = nullptr;
  WifiManager* wifi = nullptr;
  WeatherService* weather = nullptr;
  TimeService* time = nullptr;
  uint32_t bootMs = 0;
};

}  // namespace deskbot
