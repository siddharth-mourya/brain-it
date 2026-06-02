#include <Arduino.h>
#include "config/ConfigManager.h"
#include "core/EventBus.h"
#include "core/MoodEngine.h"
#include "core/Scheduler.h"
#include "core/StateMachine.h"
#include "display/RgbEngine.h"
#include "display/SH1106Renderer.h"
#include "input/PresenceManager.h"
#include "input/TouchManager.h"
#include "network/OTAService.h"
#include "network/WifiManager.h"
#include "pages/AnimationPage.h"
#include "pages/ClockPage.h"
#include "pages/EyesPage.h"
#include "pages/PageContext.h"
#include "pages/PageManager.h"
#include "pages/SystemPage.h"
#include "pages/ThoughtPage.h"
#include "pages/WeatherPage.h"
#include "services/TimeService.h"
#include "services/WeatherService.h"

using namespace deskbot;

EventBus eventBus;
ConfigManager configManager;
Scheduler scheduler;
StateMachine stateMachine;
MoodEngine moodEngine;
SH1106Renderer renderer;
RgbEngine rgbEngine;
TouchManager touchManager;
PresenceManager presenceManager;
WifiManager wifiManager;
OTAService otaService;
TimeService timeService;
WeatherService weatherService;
PageManager pageManager;
PageContext pageContext;

EyesPage eyesPage(pageContext);
ClockPage clockPage(pageContext);
WeatherPage weatherPage(pageContext);
ThoughtPage thoughtPage(pageContext);
AnimationPage animationPage(pageContext);
SystemPage systemPage(pageContext);

void logEvent(const Event& event, void*) {
  Serial.print("event=");
  Serial.print(eventName(event.type));
  if (event.value) {
    Serial.print(" value=");
    Serial.print(event.value);
  }
  if (event.message) {
    Serial.print(" message=");
    Serial.print(event.message);
  }
  Serial.println();
}

void setupPageContext() {
  pageContext.bus = &eventBus;
  pageContext.stateMachine = &stateMachine;
  pageContext.moodEngine = &moodEngine;
  pageContext.config = &configManager;
  pageContext.wifi = &wifiManager;
  pageContext.weather = &weatherService;
  pageContext.time = &timeService;
  pageContext.bootMs = millis();
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\nDeskBot firmware " DESKBOT_FIRMWARE_VERSION);

  eventBus.subscribe(logEvent);
  bool configOk = configManager.begin();

  if (!renderer.begin()) {
    Serial.println("Display init failed; continuing for network/config debug.");
    eventBus.emit(EventType::ErrorRaised, 0, "Display init failed");
  }
  renderer.setBrightness(configManager.config().brightness);
  rgbEngine.begin();

  scheduler.begin(eventBus);
  stateMachine.begin(eventBus);
  moodEngine.begin(eventBus);
  pageManager.begin(eventBus);
  setupPageContext();

  pageManager.registerPage(&eyesPage);
  pageManager.registerPage(&clockPage);
  pageManager.registerPage(&weatherPage);
  pageManager.registerPage(&thoughtPage);
  pageManager.registerPage(&animationPage);
  pageManager.registerPage(&systemPage);

  touchManager.begin(eventBus, scheduler);
  presenceManager.begin(eventBus, scheduler);
  wifiManager.begin(eventBus, configManager);
  timeService.begin();
  weatherService.begin(eventBus, configManager);
  otaService.begin(eventBus);

  if (!configOk) eventBus.emit(EventType::ErrorRaised, 0, configManager.lastError());
  eventBus.emit(EventType::BootComplete);
}

void loop() {
  uint32_t now = millis();

  touchManager.poll(now);
  presenceManager.poll(now);
  scheduler.tick(now);
  wifiManager.tick(now);
  otaService.tick();
  timeService.tick();
  weatherService.tick(now);

  eventBus.dispatch();
  stateMachine.update(now);
  moodEngine.update(now);
  pageManager.update(now);

  renderer.beginFrame();
  pageManager.render(renderer);
  renderer.flush();
  rgbEngine.update(now, stateMachine.state(), moodEngine.snapshot());

  delay(8);
}
