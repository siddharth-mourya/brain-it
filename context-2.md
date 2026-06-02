<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Expression Bot System Flow</title>
    <script src="https://cdn.jsdelivr.net/npm/mermaid@10/dist/mermaid.min.js"></script>
    <script src="https://cdn.tailwindcss.com"></script>
    <style>
        body {
            background-color: #fff;
            color: #121212;
        }
        .mermaid {
            background-color: #fbfbfc;
            border: 0.5px solid #f4f4f5;
            border-radius: 8px;
            padding: 16px;
        }
        .mermaid svg {
            max-width: 100%;
            height: auto;
        }
    </style>
</head>
<body class="p-4 md:p-8">
    <div class="max-w-7xl mx-auto">
        <header class="mb-8">
            <h1 class="text-3xl font-bold text-gray-900 mb-2">Expression Bot Full System Flow</h1>
            <p class="text-base text-gray-600">Complete architecture visualization from boot to runtime</p>
        </header>
        
        <div class="bg-white border border-gray-200 rounded-lg p-4 md:p-6 shadow-sm">
            <div class="mermaid">
graph TD
    A[<b>DEVICE POWER ON</b><br/>Trigger Sources: USB, Type-C, Battery, Reset] --> B{<b>BOOTLOADER</b><br/>Verify firmware, validate OTA, rollback, init memory}
    B -->|Valid Firmware| C[<b>SYSTEM INITIALIZATION</b><br/>Serial Debug, Heap, NVS, LittleFS, Display Driver, LVGL, RGB, Touch, Scheduler, State Machine, WiFi Mgr, API Client, OTA Service Init<br/>Parallel Tasks: Heap, FPS, Watchdog]
    B -->|Corrupted Firmware| B_RB[Rollback Firmware]
    B -->|No Valid Firmware| B_RM[Recovery Mode]
    B_RB --> B
    
    C --> D[<b>CONFIG LOADER</b><br/>Reads: WiFi creds, device token, owner id, bot personality, RGB theme, brightness, paired bot info, last state, animation packs<br/>Failure Handling: Fallback defaults, safe assets]
    D --> E{<b>DEVICE PROVISIONED ?</b>}
    E -->|NO| F[<b>PAIRING MODE</b><br/>Start SoftAP/Captive Portal, Generate Pairing Code, Wait for WiFi, Register Device, Receive Device Token, Store Secure Creds<br/>States: WAITING_WIFI, CONNECTING, REGISTRATION_FAILED, SUCCESS]
    E -->|YES| G[<b>NORMAL MODE</b><br/>Start Runtime Scheduler, Mood Engine, State Machine, Load Idle Personality, Init Animation Loop, Begin Frame Rendering, Start Background Services<br/>Initial State: BOOT_COMPLETE, IDLE]
    
    F --> G_Transition[Provisioning Complete]
    G_Transition --> G
    
    G --> H[<b>MAIN LOOP TICK</b><br/>30 FPS target, ~33ms loop<br/>Responsibilities: Poll inputs, process events, update states/mood, resolve animation, render frame, flush display, update LEDs]
    
    subgraph MainRuntime["Main Runtime Engine"]
        H --> I[<b>INPUT MANAGER</b><br/>Hardware: Touch, Future Buttons, Charging State<br/>System: WiFi Connected, OTA Available, Low Memory<br/>Timer: Blink, Inactivity, Cooldown, Random Idle<br/>Processing: Debounce, Tap, Long Press, Double Tap]
        I --> J[<b>EVENT BUS</b><br/>Standardized Internal Event Layer<br/>Examples: TOUCH_SINGLE, WIFI_CONNECTED, BLINK_TRIGGER, CONFIG_UPDATED]
        J --> K[<b>PRIORITY MANAGER</b><br/>Resolves Competing States<br/>Table: OTA 100 - PAIRING 90 - ERROR 80 - USER INTERACTION 70 - NETWORK 60 - MOOD 50 - IDLE 20 - SLEEP 10]
        K --> L[<b>STATE MACHINE</b><br/>Core States: BOOTING, PAIRING, ERROR, UPDATING, IDLE, HAPPY, EXCITED, SLEEPY, CURIOUS, ANNOYED, WAKE_UP, FALL_ASLEEP<br/>Logic: IDLE + TOUCH_SINGLE → EXCITED]
        L --> M[<b>MOOD ENGINE</b><br/>Long-Term Personality Variables<br/>Vars: Happiness, Energy, Excitement, Boredom, Attention<br/>Evolution: Interaction++, Inactivity++, Night--, Spam++]
        M --> N[<b>ANIMATION RESOLVER</b><br/>Determines Which Animation To Play<br/>Inputs: Current state, mood modifiers, priority, current animation, interrupt permissions<br/>Rules: IDLE+HAPPY → idle_soft_blink]
        N --> O[<b>ANIMATION PLAYER</b><br/>Runtime Playback Engine<br/>Responsibilities: Frame sequencing, FPS timing, looping, easing, transitions, interrupt handling, buffering<br/>Sources: LittleFS, cached packs<br/>Flow: selected → load → decode → push]
        O --> P[<b>RENDERER</b><br/>Converts Frames Into Screen Pixels<br/>Responsibilities: Framebuffer composition, LVGL draw, dirty rectangle opt, display flush, FPS sync<br/>Targets: Stable FPS, no flickering, low latency, minimal tearing]
        P -->|Rendered Frame| Q[<b>DISPLAY OUTPUT</b><br/>Final Rendered Expression<br/>Examples: Blinking eyes, bouncing reaction, sleepy droop, happy face<br/>Refresh Rate: 30 FPS typical, partial redraw opt]
        P -->|RGB Data| R[<b>RGB ENGINE</b><br/>Ambient LED Personality<br/>Behaviors: Breathing, pulsing, smooth fades, reactive flashes<br/>Synced With: Mood, animations]
    end
    
    subgraph BackgroundServices["Background Services"]
        S[<b>SCHEDULER</b><br/>Generates Timed Runtime Events<br/>Examples: Blink 5-12s, idle reaction 2min, sleepy inactivity, sync config 5min, reconnect WiFi 30s]
        T[<b>STORAGE SYSTEM</b><br/>Persistent Device Storage<br/>Uses: NVS, LittleFS<br/>Stores: WiFi creds, auth tokens, user prefs, assets, cached configs, brightness]
        U[<b>NETWORK MANAGER</b><br/>Cloud Communication Layer<br/>Responsibilities: WiFi conn, reconnect, websocket, REST API, cloud heartbeat, config sync]
        V[<b>OTA SYSTEM</b><br/>Firmware Update Pipeline<br/>Flow: Check version → download → checksum → flash inactive → switch → reboot]
    end
    
    S -.->|Triggers| H
    U -.->|Events| J
    V -.->|State Change| L
    T -.->|Load/Save| D
    
    style A fill:#3ab5f7,stroke:#1e96eb,stroke-width:2px,color:#fff
    style E fill:#f4f4f5,stroke:#8e8d91,stroke-width:2px
    style B fill:#f4f4f5,stroke:#8e8d91,stroke-width:2px
    style MainRuntime fill:#fbfbfc,stroke:#1e96eb,stroke-width:1px
    style BackgroundServices fill:#fbfbfc,stroke:#1e96eb,stroke-width:1px
            </div>
        </div>
        
        <footer class="mt-8 text-center text-sm text-gray-500">
            <p>System Architecture Diagram • Generated with Mermaid.js</p>
        </footer>
    </div>
    
    <script>
        mermaid.initialize({ 
            startOnLoad: true,
            theme: 'base',
            themeVariables: {
                primaryColor: '#3ab5f7',
                primaryTextColor: '#121212',
                primaryBorderColor: '#1e96eb',
                lineColor: '#8e8d91',
                secondaryColor: '#f4f4f5',
                tertiaryColor: '#fbfbfc',
                fontSize: '14px',
                fontFamily: 'system-ui, -apple-system, sans-serif'
            },
            flowchart: {
                curve: 'basis',
                padding: 12,
                nodeSpacing: 50,
                rankSpacing: 50
            }
        });
    </script>
</body>
</html>
