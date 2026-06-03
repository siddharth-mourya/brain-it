/*************************************************
 * RobotBuddy
 * Part 1 - Core Framework
 *
 * Features:
 * - OLED Setup
 * - Mood System
 * - Animation System
 * - Pet Stats
 * - Runtime Statistics
 * - Screen Modes
 * - Button Framework
 * - Thought System
 * - Core Timers
 *
 * Compatible With:
 * ESP8266 NodeMCU
 * SH1106 OLED
 * Adafruit_SH110X
 * Irisoled
 * IrisoledAnimation
 *************************************************/

#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

#include <ArduinoJson.h>
#include <LittleFS.h>

#include <time.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <Irisoled.h>
#include <IrisoledAnimation.h>

#define BUTTON_PIN D5

// =====================================================
// OLED
// =====================================================

Adafruit_SH1106G display(128, 64, &Wire, -1);

// =====================================================
// WEB SERVER
// =====================================================

ESP8266WebServer server(80);

// =====================================================
// CONFIGURATION
// =====================================================

struct Config {

  String ssid;
  String password;

  String city;

  float latitude;
  float longitude;

  int sleepStartHour;
  int sleepEndHour;

};

Config config;

// =====================================================
// DIGITAL PET
// =====================================================

struct PetStats {

  int happiness;
  int energy;
  int curiosity;

};

PetStats pet = {
  80,
  80,
  80
};

// =====================================================
// RUNTIME STATS
// =====================================================

struct RuntimeStats {

  uint32_t bootMillis;
  uint32_t weatherUpdates;
  uint32_t buttonPresses;

};

RuntimeStats stats;

// =====================================================
// WEATHER DATA
// =====================================================

float temperature = 0;
float windSpeed = 0;

int weatherCode = 0;

String weatherText = "Loading";

// =====================================================
// THOUGHTS
// =====================================================

const char* thoughts[] = {

  "Need Coffee",
  "Keep Coding",
  "Ship It",
  "Build Something",
  "Hello Human",
  "Push To Prod?",
  "404 Motivation",
  "Refactor Time",
  "Deploy Friday?",
  "Need More RAM"

};

const uint8_t THOUGHT_COUNT =
  sizeof(thoughts) /
  sizeof(thoughts[0]);

String currentThought =
  "Hello Human";

// =====================================================
// SCREEN MODES
// =====================================================

enum ScreenMode {

  FACE_MODE,
  INFO_MODE,
  STATS_MODE,
  THOUGHT_MODE

};

ScreenMode currentMode =
  FACE_MODE;

// =====================================================
// MOODS
// =====================================================

enum Mood {

  MOOD_HAPPY,
  MOOD_CURIOUS,
  MOOD_THINKING,
  MOOD_FOCUSED,
  MOOD_SLEEPY,
  MOOD_ANGRY,
  MOOD_BORED

};

Mood currentMood =
  MOOD_HAPPY;

// =====================================================
// HAPPY ANIMATION
// =====================================================

const unsigned char* happyFrames[] = {

  Irisoled::happy,
  Irisoled::blink,
  Irisoled::happy,
  Irisoled::wink_left,
  Irisoled::happy,
  Irisoled::wink_right

};

const uint16_t happyDelays[] = {

  2500,
  120,
  1500,
  250,
  1500,
  250

};

IrisoledAnimation happyAnim(
  happyFrames,
  sizeof(happyFrames) /
    sizeof(happyFrames[0]),
  happyDelays,
  200,
  true
);

// =====================================================
// CURIOUS
// =====================================================

const unsigned char* curiousFrames[] = {

  Irisoled::normal,
  Irisoled::look_left,
  Irisoled::normal,
  Irisoled::look_right,
  Irisoled::normal,
  Irisoled::look_up,
  Irisoled::normal

};

const uint16_t curiousDelays[] = {

  1200,
  800,
  1200,
  800,
  1200,
  800,
  1200

};

IrisoledAnimation curiousAnim(
  curiousFrames,
  sizeof(curiousFrames) /
    sizeof(curiousFrames[0]),
  curiousDelays,
  200,
  true
);

// =====================================================
// THINKING
// =====================================================

const unsigned char* thinkingFrames[] = {

  Irisoled::focused,
  Irisoled::look_left,
  Irisoled::focused,
  Irisoled::look_right,
  Irisoled::focused

};

const uint16_t thinkingDelays[] = {

  1500,
  800,
  1500,
  800,
  1500

};

IrisoledAnimation thinkingAnim(
  thinkingFrames,
  sizeof(thinkingFrames) /
    sizeof(thinkingFrames[0]),
  thinkingDelays,
  200,
  true
);

// =====================================================
// FOCUSED
// =====================================================

const unsigned char* focusedFrames[] = {

  Irisoled::focused,
  Irisoled::blink,
  Irisoled::focused

};

const uint16_t focusedDelays[] = {

  2500,
  120,
  2500

};

IrisoledAnimation focusedAnim(
  focusedFrames,
  sizeof(focusedFrames) /
    sizeof(focusedFrames[0]),
  focusedDelays,
  200,
  true
);

// =====================================================
// SLEEPY
// =====================================================

const unsigned char* sleepyFrames[] = {

  Irisoled::sleepy,
  Irisoled::blink,
  Irisoled::sleepy

};

const uint16_t sleepyDelays[] = {

  4000,
  500,
  4000

};

IrisoledAnimation sleepyAnim(
  sleepyFrames,
  sizeof(sleepyFrames) /
    sizeof(sleepyFrames[0]),
  sleepyDelays,
  200,
  true
);

// =====================================================
// ANGRY
// =====================================================

const unsigned char* angryFrames[] = {

  Irisoled::angry,
  Irisoled::furious,
  Irisoled::angry

};

const uint16_t angryDelays[] = {

  1000,
  600,
  1000

};

IrisoledAnimation angryAnim(
  angryFrames,
  sizeof(angryFrames) /
    sizeof(angryFrames[0]),
  angryDelays,
  200,
  true
);

// =====================================================
// BORED
// =====================================================

const unsigned char* boredFrames[] = {

  Irisoled::bored,
  Irisoled::blink,
  Irisoled::bored

};

const uint16_t boredDelays[] = {

  3500,
  120,
  3500

};

IrisoledAnimation boredAnim(
  boredFrames,
  sizeof(boredFrames) /
    sizeof(boredFrames[0]),
  boredDelays,
  200,
  true
);

// =====================================================
// ACTIVE ANIMATION POINTER
// =====================================================

IrisoledAnimation* activeAnim =
  &happyAnim;

// =====================================================
// TIMERS
// =====================================================

unsigned long lastWeatherUpdate = 0;
unsigned long lastMoodUpdate = 0;
unsigned long lastThoughtTime = 0;
unsigned long lastPetUpdate = 0;

unsigned long screenStartTime = 0;

// =====================================================
// BUTTON STATE
// =====================================================

bool lastButtonState = HIGH;

unsigned long buttonDownTime = 0;

bool longPressHandled = false;

const unsigned long LONG_PRESS_MS = 1500;

// =====================================================
// MOOD HELPER
// =====================================================

void setMood(Mood mood) {

  currentMood = mood;

  switch (mood) {

    case MOOD_HAPPY:
      activeAnim = &happyAnim;
      break;

    case MOOD_CURIOUS:
      activeAnim = &curiousAnim;
      break;

    case MOOD_THINKING:
      activeAnim = &thinkingAnim;
      break;

    case MOOD_FOCUSED:
      activeAnim = &focusedAnim;
      break;

    case MOOD_SLEEPY:
      activeAnim = &sleepyAnim;
      break;

    case MOOD_ANGRY:
      activeAnim = &angryAnim;
      break;

    case MOOD_BORED:
      activeAnim = &boredAnim;
      break;
  }

  activeAnim->reset();
  activeAnim->start();
}

// =====================================================
// DRAW FACE MODE
// =====================================================

void drawFaceMode() {

  activeAnim->update(
    display,
    0,
    0,
    128,
    64
  );
}

// =====================================================
// CORE SETUP
// =====================================================

void setupCore() {

  pinMode(
    BUTTON_PIN,
    INPUT_PULLUP
  );

  stats.bootMillis =
    millis();

  setMood(
    MOOD_HAPPY
  );
}


/*************************************************
 * PART 2
 * WIFI + NTP + WEATHER + SLEEP MODE
 *************************************************/

// =====================================================
// WEATHER URL
// =====================================================

String weatherURL;

// =====================================================
// WIFI
// =====================================================

void connectWiFi() {

  Serial.println("Connecting WiFi...");

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    config.ssid.c_str(),
    config.password.c_str()
  );

  unsigned long start =
    millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - start < 20000
  ) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();

  if (
    WiFi.status() == WL_CONNECTED
  ) {

    Serial.println("WiFi Connected");

    Serial.print("IP: ");
    Serial.println(
      WiFi.localIP()
    );
  }
}

// =====================================================
// NTP
// =====================================================

void initTime() {

  configTime(
    19800,
    0,
    "pool.ntp.org",
    "time.nist.gov",
    "time.google.com"
  );

  Serial.println(
    "Waiting for NTP..."
  );

  time_t now =
    time(nullptr);

  int retries = 0;

  while (
    now < 100000 &&
    retries < 20
  ) {

    delay(500);

    now =
      time(nullptr);

    retries++;
  }

  Serial.println(
    "Time Synced"
  );
}

// =====================================================
// BUILD WEATHER URL
// =====================================================

void buildWeatherURL() {

  weatherURL =
    "http://api.open-meteo.com/v1/forecast?"
    "latitude=" +
    String(config.latitude, 4) +
    "&longitude=" +
    String(config.longitude, 4) +
    "&current_weather=true";
}

// =====================================================
// WEATHER CODE TEXT
// =====================================================

String decodeWeather(
  int code
) {

  if (code == 0)
    return "Clear";

  if (code <= 3)
    return "Cloudy";

  if (code < 60)
    return "Fog";

  if (code < 70)
    return "Rain";

  if (code < 100)
    return "Storm";

  return "Unknown";
}

// =====================================================
// WEATHER -> MOOD
// =====================================================

void applyWeatherMood() {

  if (
    weatherText == "Clear"
  ) {

    setMood(
      MOOD_HAPPY
    );

  } else if (
    weatherText == "Cloudy"
  ) {

    setMood(
      MOOD_CURIOUS
    );

  } else if (
    weatherText == "Rain"
  ) {

    setMood(
      MOOD_BORED
    );

  } else if (
    weatherText == "Storm"
  ) {

    setMood(
      MOOD_ANGRY
    );
  }
}

// =====================================================
// FETCH WEATHER
// =====================================================

void fetchWeather() {

  if (
    WiFi.status() !=
    WL_CONNECTED
  )
    return;

  WiFiClient client;
  HTTPClient http;

  http.begin(
    client,
    weatherURL
  );

  int code =
    http.GET();

  if (code == 200) {

    String payload =
      http.getString();

    JsonDocument doc;

    auto err =
      deserializeJson(
        doc,
        payload
      );

    if (!err) {

      temperature =
        doc["current_weather"]
           ["temperature"];

      windSpeed =
        doc["current_weather"]
           ["windspeed"];

      weatherCode =
        doc["current_weather"]
           ["weathercode"];

      weatherText =
        decodeWeather(
          weatherCode
        );

      stats.weatherUpdates++;

      applyWeatherMood();

      Serial.print(
        "Temp: "
      );

      Serial.println(
        temperature
      );
    }
  }

  http.end();
}

// =====================================================
// CURRENT HOUR
// =====================================================

int currentHour() {

  time_t now =
    time(nullptr);

  struct tm* t =
    localtime(&now);

  return t->tm_hour;
}

// =====================================================
// SLEEP MODE CHECK
// =====================================================

bool isSleeping() {

  int hour =
    currentHour();

  if (
    config.sleepStartHour >
    config.sleepEndHour
  ) {

    return (
      hour >=
        config.sleepStartHour ||
      hour <
        config.sleepEndHour
    );
  }

  return (
    hour >=
      config.sleepStartHour &&
    hour <
      config.sleepEndHour
  );
}

// =====================================================
// AUTO MOOD SYSTEM
// =====================================================

void updateMoodSystem() {

  static bool wasSleeping =
    false;

  bool sleeping =
    isSleeping();

  if (
    sleeping &&
    !wasSleeping
  ) {

    setMood(
      MOOD_SLEEPY
    );

    wasSleeping =
      true;

    return;
  }

  if (
    !sleeping &&
    wasSleeping
  ) {

    wasSleeping =
      false;

    applyWeatherMood();
  }
}

// =====================================================
// DIGITAL PET
// =====================================================

void updatePetStats() {

  if (
    millis() -
      lastPetUpdate <
    60000UL
  )
    return;

  lastPetUpdate =
    millis();

  pet.energy--;

  if (
    weatherText ==
    "Clear"
  ) {

    pet.happiness++;

  } else if (
    weatherText ==
    "Rain"
  ) {

    pet.happiness--;
  }

  pet.curiosity++;

  pet.happiness =
    constrain(
      pet.happiness,
      0,
      100
    );

  pet.energy =
    constrain(
      pet.energy,
      0,
      100
    );

  pet.curiosity =
    constrain(
      pet.curiosity,
      0,
      100
    );
}

// =====================================================
// PERIODIC TASKS
// =====================================================

void runBackgroundTasks() {

  if (
    millis() -
      lastWeatherUpdate >
    600000UL
  ) {

    fetchWeather();

    lastWeatherUpdate =
      millis();
  }

  if (
    millis() -
      lastMoodUpdate >
    10000UL
  ) {

    updateMoodSystem();

    lastMoodUpdate =
      millis();
  }

  updatePetStats();
}

/*************************************************
 * PART 3
 * OLED SCREENS + BUTTON CONTROLS
 *************************************************/

// =====================================================
// TIME STRING
// =====================================================

String getTimeString() {

  time_t now =
    time(nullptr);

  struct tm* t =
    localtime(&now);

  char buf[20];

  sprintf(
    buf,
    "%02d:%02d:%02d",
    t->tm_hour,
    t->tm_min,
    t->tm_sec
  );

  return String(buf);
}

// =====================================================
// UPTIME
// =====================================================

String getUptimeString() {

  uint32_t sec =
    (millis() -
     stats.bootMillis) /
    1000;

  uint32_t hrs =
    sec / 3600;

  uint32_t mins =
    (sec % 3600) / 60;

  sec %= 60;

  char buf[20];

  sprintf(
    buf,
    "%02lu:%02lu:%02lu",
    hrs,
    mins,
    sec
  );

  return String(buf);
}

// =====================================================
// INFO SCREEN
// =====================================================

void drawInfoScreen() {

  display.clearDisplay();

  display.setTextColor(
    SH110X_WHITE
  );

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print(config.city);

  display.drawLine(
    0,
    10,
    127,
    10,
    SH110X_WHITE
  );

  display.setTextSize(2);

  display.setCursor(
    0,
    15
  );

  display.print(
    getTimeString()
  );

  display.setTextSize(1);

  display.setCursor(
    0,
    45
  );

  display.print(
    temperature,
    1
  );

  display.print(" C");

  display.setCursor(
    70,
    45
  );

  display.print(
    weatherText
  );

  display.display();
}

// =====================================================
// STATS SCREEN
// =====================================================

void drawStatsScreen() {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("UP:");
  display.print(
    getUptimeString()
  );

  display.setCursor(0, 10);
  display.print("RSSI:");
  display.print(
    WiFi.RSSI()
  );

  display.setCursor(0, 20);
  display.print("W:");
  display.print(
    stats.weatherUpdates
  );

  display.setCursor(0, 30);
  display.print("BTN:");
  display.print(
    stats.buttonPresses
  );

  display.setCursor(0, 40);
  display.print(
    WiFi.localIP()
  );

  display.display();
}

// =====================================================
// THOUGHT SCREEN
// =====================================================

void drawThoughtScreen() {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(
    20,
    0
  );

  display.print(
    "THOUGHT"
  );

  display.drawLine(
    0,
    10,
    127,
    10,
    SH110X_WHITE
  );

  display.setTextSize(2);

  display.setCursor(
    0,
    25
  );

  display.print(
    currentThought
  );

  display.display();
}

// =====================================================
// RANDOM THOUGHT
// =====================================================

void showRandomThought() {

  int idx =
    random(
      THOUGHT_COUNT
    );

  currentThought =
    thoughts[idx];

  currentMode =
    THOUGHT_MODE;

  screenStartTime =
    millis();
}

// =====================================================
// AUTO THOUGHTS
// =====================================================

void updateThoughtSystem() {

  if (
    millis() -
      lastThoughtTime >
    180000UL
  ) {

    lastThoughtTime =
      millis();

    showRandomThought();
  }
}

// =====================================================
// SCREEN TIMEOUT
// =====================================================

void updateScreenTimeout() {

  if (
    currentMode ==
    FACE_MODE
  )
    return;

  if (
    millis() -
      screenStartTime >
    10000UL
  ) {

    currentMode =
      FACE_MODE;
  }
}

// =====================================================
// BUTTON SYSTEM
// =====================================================

void handleButton() {

  bool state =
    digitalRead(
      BUTTON_PIN
    );

  // pressed

  if (
    state == LOW &&
    lastButtonState ==
      HIGH
  ) {

    buttonDownTime =
      millis();

    longPressHandled =
      false;
  }

  // long press

  if (
    state == LOW &&
    !longPressHandled &&
    millis() -
        buttonDownTime >
      LONG_PRESS_MS
  ) {

    stats.buttonPresses++;

    currentMode =
      STATS_MODE;

    screenStartTime =
      millis();

    longPressHandled =
      true;
  }

  // released

  if (
    state == HIGH &&
    lastButtonState ==
      LOW
  ) {

    if (
      !longPressHandled
    ) {

      stats.buttonPresses++;

      currentMode =
        INFO_MODE;

      screenStartTime =
        millis();
    }
  }

  lastButtonState =
    state;
}

// =====================================================
// SCREEN ROUTER
// =====================================================

void updateDisplay() {

  switch (
    currentMode
  ) {

    case FACE_MODE:

      drawFaceMode();
      break;

    case INFO_MODE:

      drawInfoScreen();
      break;

    case STATS_MODE:

      drawStatsScreen();
      break;

    case THOUGHT_MODE:

      drawThoughtScreen();
      break;
  }
}

/*************************************************
 * PART 4
 * WEB DASHBOARD + REST API
 *************************************************/

// =====================================================
// MOOD TO STRING
// =====================================================

String moodToString() {

  switch (currentMood) {

    case MOOD_HAPPY:
      return "Happy";

    case MOOD_CURIOUS:
      return "Curious";

    case MOOD_THINKING:
      return "Thinking";

    case MOOD_FOCUSED:
      return "Focused";

    case MOOD_SLEEPY:
      return "Sleepy";

    case MOOD_ANGRY:
      return "Angry";

    case MOOD_BORED:
      return "Bored";
  }

  return "Unknown";
}

// =====================================================
// JSON STATUS
// =====================================================

void handleStatus() {

  JsonDocument doc;

  doc["mood"] =
    moodToString();

  doc["temperature"] =
    temperature;

  doc["weather"] =
    weatherText;

  doc["time"] =
    getTimeString();

  doc["thought"] =
    currentThought;

  doc["happiness"] =
    pet.happiness;

  doc["energy"] =
    pet.energy;

  doc["curiosity"] =
    pet.curiosity;

  doc["wifi_rssi"] =
    WiFi.RSSI();

  doc["ip"] =
    WiFi.localIP()
      .toString();

  doc["uptime"] =
    getUptimeString();

  String json;

  serializeJson(
    doc,
    json
  );

  server.send(
    200,
    "application/json",
    json
  );
}

// =====================================================
// HAPPY
// =====================================================

void handleHappy() {

  setMood(
    MOOD_HAPPY
  );

  server.send(
    200,
    "text/plain",
    "Happy"
  );
}

// =====================================================
// ANGRY
// =====================================================

void handleAngry() {

  setMood(
    MOOD_ANGRY
  );

  server.send(
    200,
    "text/plain",
    "Angry"
  );
}

// =====================================================
// SLEEPY
// =====================================================

void handleSleep() {

  setMood(
    MOOD_SLEEPY
  );

  server.send(
    200,
    "text/plain",
    "Sleepy"
  );
}

// =====================================================
// THOUGHT
// =====================================================

void handleThought() {

  showRandomThought();

  server.send(
    200,
    "text/plain",
    currentThought
  );
}

// =====================================================
// INFO SCREEN
// =====================================================

void handleInfo() {

  currentMode =
    INFO_MODE;

  screenStartTime =
    millis();

  server.send(
    200,
    "text/plain",
    "Info Screen"
  );
}

// =====================================================
// DASHBOARD PAGE
// =====================================================

String buildDashboard() {

  String html;

  html +=
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta charset='utf-8'>"
    "<meta name='viewport' "
    "content='width=device-width,"
    "initial-scale=1'>"

    "<title>RobotBuddy</title>"

    "<style>"

    "body{"
    "font-family:Arial;"
    "background:#111;"
    "color:#fff;"
    "padding:20px;"
    "}"

    "button{"
    "padding:10px;"
    "margin:5px;"
    "font-size:16px;"
    "}"

    ".card{"
    "background:#222;"
    "padding:15px;"
    "margin-bottom:10px;"
    "border-radius:8px;"
    "}"

    "</style>"
    "</head>"
    "<body>";

  html +=
    "<h1>RobotBuddy</h1>";

  html +=
    "<div class='card'>";

  html +=
    "<p><b>Mood:</b> " +
    moodToString() +
    "</p>";

  html +=
    "<p><b>Time:</b> " +
    getTimeString() +
    "</p>";

  html +=
    "<p><b>Temp:</b> " +
    String(
      temperature,
      1
    ) +
    " C</p>";

  html +=
    "<p><b>Weather:</b> " +
    weatherText +
    "</p>";

  html +=
    "<p><b>Thought:</b> " +
    currentThought +
    "</p>";

  html +=
    "<p><b>RSSI:</b> " +
    String(
      WiFi.RSSI()
    ) +
    "</p>";

  html +=
    "<p><b>Uptime:</b> " +
    getUptimeString() +
    "</p>";

  html +=
    "</div>";

  html +=
    "<div class='card'>";

  html +=
    "<h3>Pet Stats</h3>";

  html +=
    "<p>Happy: " +
    String(
      pet.happiness
    ) +
    "</p>";

  html +=
    "<p>Energy: " +
    String(
      pet.energy
    ) +
    "</p>";

  html +=
    "<p>Curiosity: " +
    String(
      pet.curiosity
    ) +
    "</p>";

  html +=
    "</div>";

  html +=
    "<button "
    "onclick=\"location.href='/happy'\">"
    "Happy"
    "</button>";

  html +=
    "<button "
    "onclick=\"location.href='/angry'\">"
    "Angry"
    "</button>";

  html +=
    "<button "
    "onclick=\"location.href='/sleep'\">"
    "Sleepy"
    "</button>";

  html +=
    "<button "
    "onclick=\"location.href='/thought'\">"
    "Thought"
    "</button>";

  html +=
    "<button "
    "onclick=\"location.href='/info'\">"
    "Info"
    "</button>";

  html +=
    "</body></html>";

  return html;
}

// =====================================================
// ROOT PAGE
// =====================================================

void handleRoot() {

  server.send(
    200,
    "text/html",
    buildDashboard()
  );
}

// =====================================================
// WEB ROUTES
// =====================================================

void setupWebServer() {

  server.on(
    "/",
    handleRoot
  );

  server.on(
    "/status",
    handleStatus
  );

  server.on(
    "/happy",
    handleHappy
  );

  server.on(
    "/angry",
    handleAngry
  );

  server.on(
    "/sleep",
    handleSleep
  );

  server.on(
    "/thought",
    handleThought
  );

  server.on(
    "/info",
    handleInfo
  );

  server.on(
  "/config",
  handleConfig
  );

  server.on(
    "/saveconfig",
    handleSaveConfig
  );

  server.begin();

  Serial.println(
    "Web Server Started"
  );

  Serial.print(
    "Open: http://"
  );

  Serial.println(
    WiFi.localIP()
  );
}

/*************************************************
 * PART 5
 * LITTLEFS CONFIG PORTAL
 *************************************************/

#define CONFIG_FILE "/config.json"

// =====================================================
// DEFAULT CONFIG
// =====================================================

void createDefaultConfig() {

  config.ssid = "Om sai";
  config.password = "";

  config.city = "Bhopal";

  config.latitude = 23.2599;
  config.longitude = 77.4126;

  config.sleepStartHour = 23;
  config.sleepEndHour = 7;
}

// =====================================================
// SAVE CONFIG
// =====================================================

bool saveConfig() {

  File file =
    LittleFS.open(
      CONFIG_FILE,
      "w"
    );

  if (!file)
    return false;

  JsonDocument doc;

  doc["ssid"] =
    config.ssid;

  doc["password"] =
    config.password;

  doc["city"] =
    config.city;

  doc["latitude"] =
    config.latitude;

  doc["longitude"] =
    config.longitude;

  doc["sleepStartHour"] =
    config.sleepStartHour;

  doc["sleepEndHour"] =
    config.sleepEndHour;

  serializeJson(
    doc,
    file
  );

  file.close();

  return true;
}

// =====================================================
// LOAD CONFIG
// =====================================================

bool loadConfig() {

  if (
    !LittleFS.exists(
      CONFIG_FILE
    )
  ) {

    createDefaultConfig();

    saveConfig();

    return true;
  }

  File file =
    LittleFS.open(
      CONFIG_FILE,
      "r"
    );

  if (!file)
    return false;

  JsonDocument doc;

  auto err =
    deserializeJson(
      doc,
      file
    );

  file.close();

  if (err)
    return false;

  config.ssid =
    doc["ssid"] |
    "";

  config.password =
    doc["password"] |
    "";

  config.city =
    doc["city"] |
    "Bhopal";

  config.latitude =
    doc["latitude"] |
    23.2599;

  config.longitude =
    doc["longitude"] |
    77.4126;

  config.sleepStartHour =
    doc["sleepStartHour"] |
    23;

  config.sleepEndHour =
    doc["sleepEndHour"] |
    7;

  return true;
}

// =====================================================
// CONFIG PAGE
// =====================================================

String buildConfigPage() {

  String html;

  html +=
    "<html>"
    "<head>"
    "<title>RobotBuddy Config</title>"
    "</head>"
    "<body>";

  html +=
    "<h2>RobotBuddy Config</h2>";

  html +=
    "<form action='/saveconfig'>";

  html +=
    "SSID:<br>"
    "<input name='ssid' value='" +
    config.ssid +
    "'><br><br>";

  html +=
    "Password:<br>"
    "<input name='password' value='" +
    config.password +
    "'><br><br>";

  html +=
    "City:<br>"
    "<input name='city' value='" +
    config.city +
    "'><br><br>";

  html +=
    "Latitude:<br>"
    "<input name='lat' value='" +
    String(
      config.latitude,
      4
    ) +
    "'><br><br>";

  html +=
    "Longitude:<br>"
    "<input name='lon' value='" +
    String(
      config.longitude,
      4
    ) +
    "'><br><br>";

  html +=
    "Sleep Start Hour:<br>"
    "<input name='sleepstart' value='" +
    String(
      config.sleepStartHour
    ) +
    "'><br><br>";

  html +=
    "Sleep End Hour:<br>"
    "<input name='sleepend' value='" +
    String(
      config.sleepEndHour
    ) +
    "'><br><br>";

  html +=
    "<input "
    "type='submit' "
    "value='Save'>";

  html +=
    "</form>";

  html +=
    "<br><br>";

  html +=
    "<a href='/'>Back</a>";

  html +=
    "</body></html>";

  return html;
}

// =====================================================
// CONFIG PAGE ROUTE
// =====================================================

void handleConfig() {

  server.send(
    200,
    "text/html",
    buildConfigPage()
  );
}

// =====================================================
// SAVE CONFIG ROUTE
// =====================================================

void handleSaveConfig() {

  if (
    server.hasArg(
      "ssid"
    )
  ) {

    config.ssid =
      server.arg(
        "ssid"
      );
  }

  if (
    server.hasArg(
      "password"
    )
  ) {

    config.password =
      server.arg(
        "password"
      );
  }

  if (
    server.hasArg(
      "city"
    )
  ) {

    config.city =
      server.arg(
        "city"
      );
  }

  if (
    server.hasArg(
      "lat"
    )
  ) {

    config.latitude =
      server.arg(
        "lat"
      ).toFloat();
  }

  if (
    server.hasArg(
      "lon"
    )
  ) {

    config.longitude =
      server.arg(
        "lon"
      ).toFloat();
  }

  if (
    server.hasArg(
      "sleepstart"
    )
  ) {

    config.sleepStartHour =
      server.arg(
        "sleepstart"
      ).toInt();
  }

  if (
    server.hasArg(
      "sleepend"
    )
  ) {

    config.sleepEndHour =
      server.arg(
        "sleepend"
      ).toInt();
  }

  saveConfig();

  buildWeatherURL();

  server.send(
    200,
    "text/html",
    "<h2>Saved</h2>"
    "<a href='/config'>Back</a>"
  );
}

// =====================================================
// LITTLEFS INIT
// =====================================================

void initStorage() {

  if (
    !LittleFS.begin()
  ) {

    Serial.println(
      "LittleFS Mount Failed"
    );

    return;
  }

  loadConfig();

  Serial.println(
    "Config Loaded"
  );
}

// =====================================================
// ARDUINO SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  Wire.begin(
    D2,
    D1
  );

  if (!display.begin(
        0x3C,
        true
      )) {

    while (true);
  }

  display.clearDisplay();
  display.display();

  setupCore();

// Temporary config
initStorage();

connectWiFi();

initTime();

buildWeatherURL();

fetchWeather();

lastWeatherUpdate =
  millis();

  setupWebServer();

  Serial.println(
    "RobotBuddy Core Started"
  );
}



// =====================================================
// ARDUINO LOOP
// =====================================================
void loop() {

  handleButton();

  runBackgroundTasks();

  updateThoughtSystem();

  updateScreenTimeout();
 
  server.handleClient();

  updateDisplay();
}