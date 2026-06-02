Great. Then I'd suggest we treat this as a proper product engineering project rather than a growing Arduino sketch.

### Phase 1 — Lock the V1 Scope

**Hardware**

* ESP32-C3 Super Mini
* 0.96" SH1106 OLED (development)
* Optional GC9A01 support (architecture ready from day one)
* Touch sensor
* RCWL-0516 (or later upgrade to VL53L0X)
* Single RGB LED
* USB-C power

**Core Features**

1. Robot Eyes
2. Clock + Date
3. Weather
4. Thought of the Day
5. GIF/Animation Viewer
6. System Page
7. WiFi Configuration Portal
8. OTA Updates

Everything else becomes a plugin later.

---

### Phase 2 — Define User Interaction

I'd keep it extremely simple:

| Action              | Result         |
| ------------------- | -------------- |
| Single Tap          | Next Page      |
| Double Tap          | Previous Page  |
| Long Press (4s)     | System Page    |
| Presence Detected   | Wake Animation |
| No Presence (5 min) | Sleep Mode     |

No knobs.
No multiple buttons.
No menu trees.

---

### Phase 3 — Project Structure

I would build it like this:

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

    WeatherPage.h
    WeatherPage.cpp

    EyesPage.h
    EyesPage.cpp

    ThoughtPage.h
    ThoughtPage.cpp

    SystemPage.h
    SystemPage.cpp

services/
    WeatherService.h
    WeatherService.cpp

    ThoughtService.h
    ThoughtService.cpp

input/
    TouchManager.h
    TouchManager.cpp

    PresenceManager.h
    PresenceManager.cpp

network/
    WifiManager.h
    WifiManager.cpp

    OTAService.h
    OTAService.cpp

config/
    ConfigManager.h
    ConfigManager.cpp

assets/
    icons/
    faces/
    gifs/
```

---

### Phase 4 — Page Plugin System

Every page follows:

```cpp
class IPage {
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual const char* name() = 0;
};
```

Weather:

```cpp
class WeatherPage : public IPage
```

Thoughts:

```cpp
class ThoughtPage : public IPage
```

News:

```cpp
class NewsPage : public IPage
```

Later enabling/disabling pages becomes:

```cpp
pageManager.registerPage(new WeatherPage());
pageManager.registerPage(new ThoughtPage());

// pageManager.registerPage(new NewsPage());
```

---

### Phase 5 — Display Independence

The biggest architectural decision:

Pages must **never** talk directly to:

* Adafruit_SH110X
* TFT_eSPI
* U8G2
* LovyanGFX

Instead:

```cpp
renderer.drawText(...)
renderer.drawBitmap(...)
renderer.drawCircle(...)
```

This allows:

```text
SH1106
↓
GC9A01
↓
Future Display
```

without touching page code.

---

### Phase 6 — Make the Bot Feel Alive

This is where most makers underestimate the value.

Instead of 20 information screens:

Create personality states:

```text
BOOTING
IDLE
HAPPY
THINKING
SLEEPING
SURPRISED
ERROR
```

Then:

```text
Presence detected
→ HAPPY

No user for 10 min
→ SLEEPING

Weather update received
→ THINKING

WiFi disconnected
→ ERROR
```

The display should reflect state first, information second.

---

### Phase 7 — Future Plugins

Later we add:

```text
plugins/

NewsPage
StockPage
CricketPage
HoroscopePage
YoutubeStatsPage
InstagramStatsPage
```

Each plugin only needs:

```cpp
init()
update()
render()
```

and can be removed without affecting anything else.

---

My recommendation is that the next thing we do is create a **full PlatformIO project skeleton** with all folders, interfaces, page manager, event bus, renderer abstraction, touch manager, and example pages. That will give you a scalable foundation before you add more features.
