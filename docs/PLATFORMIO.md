# PlatformIO quick guide

This project uses PlatformIO with the Arduino framework for the ESP32-C3.

Layout

- `platformio.ini` — build configuration and optional library dependencies.
- `src/` — application source files.
- `lib/` — (optional) local libraries.

Basic commands

- Build: `platformio run`
- Upload to device: `platformio run -t upload`
- Monitor serial: `platformio device monitor -b 115200`

Notes

- If you enable a real display driver (SH1106 or GC9), uncomment the
  corresponding `lib_deps` in `platformio.ini` or add them to your global
  PlatformIO library list.
- To enable driver-specific compilation flags, add `build_flags = -DUSE_SH1106`
  (or `-DUSE_GC9`) under the `[env:...]` section in `platformio.ini`.
