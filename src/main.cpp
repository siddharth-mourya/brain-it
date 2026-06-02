#include <Arduino.h>
#include "core/EventBus.h"
#include "core/Scheduler.h"
#include "core/StateMachine.h"
#include "display/DummyRenderer.h"
#include "display/SH1106Renderer.h"
#include "display/GC9Renderer.h"
#include "pages/PageManager.h"
#include "pages/EyesPage.h"
#include "pages/ClockPage.h"
#include "pages/WeatherPage.h"
#include "pages/ThoughtPage.h"
#include "pages/SystemPage.h"
#include "config/ConfigManager.h"
#include "services/TimeService.h"
#include "services/WeatherService.h"
#include "ota/OtaManager.h"
#include "network/WifiManager.h"

EventBus gEventBus;
Scheduler gScheduler;
StateMachine gStateMachine;
PageManager gPageManager;
WifiManager gWifiManager;

// Optional services
TimeService gTimeService;
WeatherService gWeatherService;
OtaManager gOtaManager;

// Renderer is selectable — create one instance in setup below by
// uncommenting the implementation you want to use.
Renderer* gRenderer = nullptr;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("DeskBot: starting...");

  gEventBus.emit(EventType::BOOT_COMPLETE);

  // Init core modules
  gScheduler.begin(&gEventBus);
  gStateMachine.begin(&gEventBus);
  gWifiManager.begin(&gEventBus);

  // Register pages (example set). More pages can be added as needed.
  gPageManager.registerPage(new EyesPage());
#if defined(USE_TIME)
  gPageManager.registerPage(new ClockPage(&gTimeService));
#endif
#if defined(USE_WEATHER)
  Config cfg = ConfigManager::instance().load();
  gPageManager.registerPage(new WeatherPage(&gWeatherService, cfg.latitude, cfg.longitude));
#endif
  gPageManager.registerPage(new ThoughtPage());
  gPageManager.registerPage(new SystemPage(&gStateMachine));
  gPageManager.initPages();

  // Choose renderer implementation by uncommenting one line below.
  // Default: Dummy (no external libs required)
  gRenderer = new DummyRenderer();

  // To use SH1106 driver (requires Adafruit_SH110X library),
  // uncomment the following line and comment the DummyRenderer line above:
  // gRenderer = new SH1106Renderer();

  // To use GC9 driver (requires TFT_eSPI or equivalent),
  // uncomment the following line and comment the DummyRenderer line above:
  // gRenderer = new GC9Renderer();

  // Start WiFi connect in background (real if enabled)
  gWifiManager.connect();

  // Initialize optional services
#if defined(USE_OTA)
  gOtaManager.begin();
#endif
#if defined(USE_TIME)
  gTimeService.begin(&gEventBus);
#endif
#if defined(USE_WEATHER)
  gWeatherService.begin(&gEventBus);
#endif
}

void loop() {
  gScheduler.tick();
  gEventBus.dispatch();
  gStateMachine.update();
  gPageManager.update();

  if (gRenderer) {
    gRenderer->beginFrame();
    gPageManager.render(*gRenderer);
    gRenderer->flush();
  }

  delay(10);
}
