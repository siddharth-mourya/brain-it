Step 2 --- Scan and open website for initial config
Before provision if user try to edit these.. dont allow
Config for Bot to be stored in Bot
Enter users name/petname
Bot Name
Personality - Hitler / Male / Female
Enter location where bot lives. --> lat / long
Gender - M / F
mode - Happy / Focus
Step 3 - show animation/ interactions/further screens
Eyes/face expressions
Games - bot playing
Identify the drawing ( skribbl )
Messaging friends ( Later during connected bots)
Costing

Square LED
led display - 250
casing - 250
esp - 250
battery - 100
charging ckt - 100
PCB - 100
touch sensor - 10
IR sensor (LD2410 , LD2450 ) - 200
Other costs (Ads, Operations) - 300
TOTAL product and component cost - 1260
Including Misc - 1600
Target Building cost -
Selling price - 1800 - 2000

Round LED
led display - 350
casing - 250
esp - 250
battery - 100
charging ckt - 100
PCB - 100
touch sensor - 10
IR sensor (LD2410 , LD2450 ) - 200
Other costs (Ads, Operations) - 300
TOTAL product and component cost - 1360
Including Misc - 1700
Selling price - 2200

Master Flow

INPUTS
↓
EVENTS
↓
STATE MACHINE
↓
ANIMATION RESOLVER
↓
RENDER PIPELINE
↓
DISPLAY + RGB OUTPUT

Product Flow

Hardware Inputs
Touch
Human presence - RCWL-0516

Config
User can select if he wants to see the news or not, and set city preference.

Modes
Weather MODE - Time, location, Date, Day, Weather Type icons
Rotating text - Temperature, Wind speed
ROBOT Eyes - using whatever libraries
System Mode - (on Long press 4 sec ) system state, high priority also shown when device boots, wifi connecting
GIF Viewer -
Thought MODE - random
Meme page - like a photo
NEWS MODE ( Optional ) - Heading of latest new based on users preference ( city )
STOCK MODE ( Optional ) - User can select up to 5 stocks to show. Limitation - needs api key
HOROSCOPE (Optional ) - User sets the zodiac first time and we show him daily horoscope
AUTO PLAY GAME - Bot will auto play some small game
Youtube / Instagram Views -
Cricket Match Score -

1. INPUT LAYER
   Responsibility
   Convert hardware/software signals into events.

---

INPUT SOURCES
Physical Inputs
touch
button
charging state
future sensors

---

System Inputs
boot complete
WiFi connected
OTA started

---

Time Inputs
idle timeout
sleep timeout
scheduled behavior

---

Network Inputs
config updated
connected bot event

---

OUTPUT OF INPUT MODULE
It should ONLY output:
events
Example:
{
"event": "TOUCH_DETECTED"
}

---

IMPORTANT
Input layer should NEVER:
directly trigger animations
directly update display
BAD architecture.

---

MODULE
/input 2. EVENT SYSTEM
VERY IMPORTANT.
This becomes:
communication layer between systems.

---

WHY?
Because:
modules should NOT tightly depend on each other.

---

Example
Touch module emits:
TOUCH_DETECTED
State machine listens.
NOT:
touch module directly calling animation engine.

---

EVENT BUS EXAMPLE
eventBus.emit(TOUCH_DETECTED);

---

POSSIBLE EVENTS

---

OUTPUT
Events are consumed by:
state machine
animation manager
RGB controller

---

MODULE
/core/event_bus

---

3. STATE MACHINE
   MOST IMPORTANT SYSTEM.
   This decides:
   what the bot CURRENTLY IS.

---

EXAMPLE STATES
System States
BOOTING
PAIRING
UPDATING
ERROR

---

Emotional States
IDLE
HAPPY
SLEEPY
EXCITED
ANNOYED

---

Transitional States
WAKE_UP
FALL_ASLEEP

---

RESPONSIBILITY
Takes:
events
timers
context
Outputs:
current active state

---

IMPORTANT
State machine should NEVER render directly.
It ONLY decides:
what should currently happen

---

EXAMPLE FLOW
Current State: IDLE

TOUCH_DETECTED event arrives

State machine:
IDLE → EXCITED

---

OUTPUT
{
"state": "EXCITED"
}

---

MODULE
/states

---

4. MOOD ENGINE
   (Optional but VERY powerful)
   This creates:
   long-term personality behavior.

---

WHY?
Without mood:
bot feels robotic.

---

Example
Repeated touch:
happiness increases
Long inactivity:
sleepiness increases

---

INTERNAL VARIABLES
{
"energy": 40,
"happiness": 75,
"attention": 20
}

---

Mood engine modifies:
animation choices
RGB themes
idle behavior

---

OUTPUT
Mood modifiers.

---

Example
{
"mood_modifier": "sleepy"
}

---

MODULE
/mood 5. ANIMATION RESOLVER
THIS is where product quality lives.

---

RESPONSIBILITY
Given:
current state
mood
priority
timers
decide:
which animation should play

---

INPUTS
{
"state": "IDLE",
"mood": "sleepy"
}

---

OUTPUT
{
"animation": "idle_sleepy_blink"
}

---

IMPORTANT
This is NOT the renderer.
This only selects animations.

---

EXAMPLE RULES

---

MODULE
/animations/resolver 6. ANIMATION ENGINE
This actually:
plays frames.

---

RESPONSIBILITY
frame timing
animation loops
transitions
frame interpolation

---

INPUT
{
"animation": "happy_bounce"
}

---

OUTPUT
Frame stream.

---

EXAMPLE
frame1
frame2
frame3

---

VERY IMPORTANT
Animation engine should support:
interruptible animations
looping animations
transition animations

---

Example
idle → touch reaction → idle
smoothly.

---

MODULE
/animations/player 7. ASSET SYSTEM
THIS is critical.

---

RESPONSIBILITY
Load animation assets from flash.

---

STORAGE STRUCTURE
/assets
/expressions
/themes
/frames

---

WHAT IS AN ASSET?
Examples:
PNG frames
compressed binary frames
theme config
RGB profile 8. RENDERER
THIS actually draws to screen.

---

RESPONSIBILITY
frame drawing
screen updates
buffering
display driver calls

---

INPUT
Frame bitmap.

---

OUTPUT
Pixels on display.

---

IMPORTANT
Renderer should know NOTHING about:
emotions
moods
touch
business logic
ONLY graphics.

---

MODULE
/display/renderer 10. SCHEDULER
VERY IMPORTANT.
This creates:
life-like timing.

---

RESPONSIBILITY
Triggers:
idle animations
random blinks
sleep mode
periodic behaviors

---

EXAMPLES
Every:
6–12 sec → blink
2 min → random reaction
10 min inactivity → sleepy

---

OUTPUT
Events.

---

MODULE
/core/scheduler 11. NETWORK MANAGER
Simple initially.

---

RESPONSIBILITY
WiFi
sync
OTA
config fetch

---

IMPORTANT
Network should NEVER control rendering directly.
It only:
emits events/config changes.

---

MODULE
/network 12. CONFIG SYSTEM
Stores:
personality
themes
brightness
behavior settings

---

INPUT
Backend/local settings.

---

OUTPUT
Runtime config values.

---

MODULE
/config
