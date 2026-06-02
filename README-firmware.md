# DeskBot Firmware

This directory is the first ESP32-C3 firmware implementation for the Markdown specifications in this repository. It is an Arduino/PlatformIO project that targets a 128x64 SH1106 OLED, a GPIO touch module, a GPIO presence sensor, WiFi provisioning, weather, NTP time, OTA, mood/state logic, and a small page system.

## Quick start

1. Install PlatformIO.
2. Connect an ESP32-C3 board.
3. Adjust pins in `platformio.ini` if your wiring differs.
4. Build the firmware:

```bash
pio run -e esp32c3
```

5. Upload the firmware:

```bash
pio run -e esp32c3 -t upload
```

6. Upload LittleFS defaults if desired:

```bash
pio run -e esp32c3 -t uploadfs
```

## Default pin map

| Signal | Default pin | Notes |
| --- | ---: | --- |
| I2C SDA | GPIO 5 | SH1106 display data. |
| I2C SCL | GPIO 6 | SH1106 display clock. |
| Touch input | GPIO 2 | Intended for a TTP223-style module, active high. |
| Presence input | GPIO 3 | Intended for RCWL/LD/VL module digital output, active high. |
| RGB LED | GPIO 8 | Single WS2812/NeoPixel-style LED. |

## Runtime behavior

- Single tap moves to the next page.
- Double tap moves to the previous page.
- A 4-second long press opens the system page.
- Presence wakes the bot and increases attention.
- If WiFi credentials are missing, the device starts an access point named `DeskBot-Setup` and serves a configuration portal at `http://192.168.4.1/`.
- Weather uses Open-Meteo and the latitude/longitude in `/config.json`.
- OTA is enabled after WiFi connects, with hostname `deskbot`.

## Configuration file

A default `data/config.json` is included for LittleFS upload. The setup portal can rewrite the same file at runtime.
