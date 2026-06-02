# Firmware Guide

The firmware should evolve from the existing prototype sketch into a modular PlatformIO or Arduino project with clear separation between core logic, display rendering, pages, services, input, network, and configuration.

## Target project structure

```text
DeskBot/
  src/
    main.cpp
    core/
      EventBus.h
      EventBus.cpp
      StateMachine.h
      StateMachine.cpp
      Scheduler.h
      Scheduler.cpp
    display/
      Renderer.h
      oled/
        SH1106Renderer.h
        SH1106Renderer.cpp
      gc9/
        GC9Renderer.h
        GC9Renderer.cpp
    pages/
      IPage.h
      PageManager.h
      WeatherPage.h
      EyesPage.h
      ThoughtPage.h
      SystemPage.h
    services/
      WeatherService.h
      ThoughtService.h
    input/
      TouchManager.h
      PresenceManager.h
    network/
      WifiManager.h
      OTAService.h
    config/
      ConfigManager.h
      ConfigManager.cpp
  assets/
    icons/
    faces/
    gifs/
```

## Page interface

Each page should implement the same lifecycle.

```cpp
class IPage {
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render(Renderer& renderer) = 0;
    virtual const char* name() const = 0;
};
```

A page manager can register pages without coupling navigation logic to page implementations.

```cpp
pageManager.registerPage(new EyesPage());
pageManager.registerPage(new WeatherPage());
pageManager.registerPage(new ThoughtPage());
pageManager.registerPage(new SystemPage());
```

## Display independence

Pages render into the abstract `Renderer`. The renderer implementation owns display-library calls.

```cpp
class Renderer {
public:
    virtual void clear() = 0;
    virtual void drawText(int x, int y, const char* text) = 0;
    virtual void drawBitmap(int x, int y, const uint8_t* data, int w, int h) = 0;
    virtual void flush() = 0;
};
```

## Main loop shape

```cpp
void loop() {
    inputManager.poll();
    scheduler.tick();
    eventBus.dispatch();
    stateMachine.update();
    moodEngine.update();
    pageManager.update();
    animationPlayer.update();
    renderer.beginFrame();
    pageManager.render(renderer);
    renderer.flush();
    networkManager.tick();
}
```

## Module responsibilities

### Input layer

Convert hardware and software signals into events.

Input sources:

- Touch.
- Optional physical button.
- Charging state.
- Presence sensor.
- Boot complete.
- WiFi connected/disconnected.
- OTA started/completed.
- Idle and sleep timers.
- Config update notifications.

Output example:

```json
{ "event": "TOUCH_SINGLE" }
```

The input layer must never trigger animations or update the display directly.

### Event bus

Provide a standardized internal communication layer.

```cpp
eventBus.emit(EventType::TouchSingle);
```

Consumers include the state machine, animation manager, RGB controller, page manager, and network/config services.

### State machine

Decide the current active state.

System states:

- `BOOTING`
- `PAIRING`
- `UPDATING`
- `ERROR`

Emotional states:

- `IDLE`
- `HAPPY`
- `SLEEPY`
- `EXCITED`
- `ANNOYED`
- `CURIOUS`

Transitional states:

- `WAKE_UP`
- `FALL_ASLEEP`

The state machine must not render directly.

### Mood engine

Maintain long-term personality behavior.

```json
{
  "energy": 40,
  "happiness": 75,
  "attention": 20,
  "boredom": 10
}
```

Mood modifies animation choices, LED themes, idle behavior, and page tone.

### Animation resolver

Map state + mood + priority + timers into an animation ID.

```json
{
  "state": "IDLE",
  "mood": "sleepy",
  "animation": "idle_sleepy_blink"
}
```

### Animation player

Play frame sequences. It handles frame timing, looping, transitions, interruptible/non-interruptible animations, and buffering or streaming frames from flash.

### Asset system

Load and version animation and UI assets.

```text
/assets
  /expressions
  /themes
  /frames
```

Supported assets can include monochrome bitmaps, compressed binary frames, theme config, RGB profiles, and future image formats.

### Renderer

Draw pixels to the display. Renderer knows graphics only. It should not know about moods, touch, weather, or business rules.

### Scheduler

Create life-like timing:

- Blink every 6–12 seconds.
- Random idle reaction every 2 minutes.
- Enter sleepy state after long inactivity.
- Sync config every 5 minutes.
- Retry WiFi every 30 seconds when disconnected.

### Network manager

Own WiFi, sync, API calls, and OTA checks. It emits events and config changes; it never controls rendering directly.

### Config system

Store and expose runtime configuration such as personality, enabled pages, theme, brightness, WiFi credentials, location, and sleep schedule.

## Prototype code notes

The original repository included a large prototype sketch named `deskbot-code.md`. It is useful as a behavior reference, but it should be refactored before becoming product firmware.

### Prototype capabilities

| Capability | Prototype behavior |
| --- | --- |
| Display | SH1106 OLED at 128×64 using `Adafruit_SH110X`. |
| Platform | ESP8266 NodeMCU-style pins. |
| Network | Connects to WiFi as station. |
| Time | NTP with fixed UTC+5:30 offset. |
| Weather | Calls Open-Meteo `current_weather` endpoint. |
| Storage | Uses LittleFS and `/config.json`. |
| Configuration UI | Local web server with dashboard and config form. |
| Mood animations | Uses `Irisoled` and `IrisoledAnimation` frames. |
| Screens | Face, info, stats, thought screens. |
| Button | Single button cycles modes; long press triggers thought/system-like behavior. |
| Stats | Tracks boot time, weather updates, button presses. |

### Data structures to preserve

```cpp
struct Config {
  String ssid;
  String password;
  String city;
  float latitude;
  float longitude;
  int sleepStartHour;
  int sleepEndHour;
};

struct PetStats {
  int happiness;
  int energy;
  int curiosity;
};

struct RuntimeStats {
  uint32_t bootMillis;
  uint32_t weatherUpdates;
  uint32_t buttonPresses;
};
```

### Weather logic

| Weather code condition | Label |
| --- | --- |
| `0` | Clear |
| `<= 3` | Cloudy |
| `< 60` | Fog |
| `< 70` | Rain |
| `< 100` | Storm |
| fallback | Unknown |

Weather-to-mood mapping:

| Weather | Mood |
| --- | --- |
| Clear | Happy |
| Cloudy | Curious |
| Rain | Bored |
| Storm | Angry |

### Refactoring risks

- Credentials appeared in the default config in the prototype and must be removed from production firmware.
- The NTP timezone offset was hard-coded for India and should become configurable.
- The web configuration form used query parameters, which can expose secrets in URLs and logs.
- Weather fetch, mood changes, and display behavior were coupled and should move through services/events.
- Long press timing differed from the V1 interaction spec.
- Direct display calls inside screen drawing should move into page classes and renderer implementations.

### Extraction map

| Prototype concern | Target module |
| --- | --- |
| `Config` load/save | `config/ConfigManager` |
| `connectWiFi` | `network/WifiManager` |
| `initTime` | `services/TimeService` or `network/TimeSync` |
| `fetchWeather` / `decodeWeather` | `services/WeatherService` |
| `setMood` / mood enums | `mood/MoodEngine` |
| Button handling | `input/TouchManager` or `input/ButtonManager` |
| Screen draw functions | `pages/*Page` |
| LittleFS initialization | `storage/StorageSystem` |
| Web routes | `network/ConfigPortal` |
