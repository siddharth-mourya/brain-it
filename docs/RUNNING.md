# How to build and run DeskBot firmware (quickstart)

1. Install PlatformIO: follow https://platformio.org/install
2. Open project folder in VS Code and use PlatformIO extension, or run from terminal.

Build and upload (example):

```bash
cd /Users/sidmoury/Documents/personal_projects/brain-it
platformio run
platformio run -t upload
platformio device monitor -b 115200
```

Switching display drivers

- Edit `src/main.cpp` and locate the renderer selection block in `setup()`.
- By default the `DummyRenderer` is used (prints to Serial).
- To use the SH1106 renderer:
  - Uncomment `gRenderer = new SH1106Renderer();` and comment out `DummyRenderer`.
  - Optionally add the Adafruit SH110X library in `platformio.ini` `lib_deps` and
    add `build_flags = -DUSE_SH1106` under the env section.
- To use the GC9 renderer, do the same with `GC9Renderer` and `-DUSE_GC9`.

Notes about drivers

- Renderers in `src/display/` default to serial-visible stubs unless the
  corresponding build flag and library are enabled. This keeps the project
  compile-safe out of the box.
