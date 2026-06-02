# Expression Bot



[Hardware Flow](https://app.affine.pro/workspace/bc5f152c-5498-4561-954d-9f57fa253fba/nmgBAURoK1GY4AHzteP19)

#### Step 2 --- Scan and open website for initial config

Before provision if user try to edit these.. dont allow

Config for Bot to be stored in Bot

1. Enter users name/petname 
2. Bot Name
3. Personality - Hitler / Male / Female
4. Enter location where bot lives. --> lat / long 
5. Gender - M / F
6. mode - Happy / Focus 

###### Step 3 - show animation/ interactions/further screens

1. Eyes/face expressions
2. Games - bot playing
3. Identify the drawing ( skribbl )
4. Messaging friends ( Later during connected bots)

# Costing



1. Square LED
   1. led display - 250
   2. casing - 250
   3. esp - 250
   4. battery - 100
   5. charging ckt - 100
   6. PCB - 100
   7. touch sensor - 10
   8. IR sensor (LD2410 , LD2450 ) - 200 
   9. Other costs (Ads, Operations)  - 300
   TOTAL product and component cost - 1260

   Including Misc - 1600

   Target Building cost - 

   Selling price - 1800 - 2000


2. Round LED
   1. led display - 350
   2. casing - 250
   3. esp - 250
   4. battery - 100
   5. charging ckt - 100
   6. PCB - 100
   7. touch sensor - 10
   8. IR sensor (LD2410 , LD2450 ) - 200 
   9. Other costs (Ads, Operations) - 300
   TOTAL product and component cost - 1360

   Including Misc - 1700

   Selling price - 2200



# Master Flow



INPUTS

&#x20;↓

EVENTS

&#x20;↓

STATE MACHINE

&#x20;↓

ANIMATION RESOLVER

&#x20;↓

RENDER PIPELINE

&#x20;↓

DISPLAY + RGB OUTPUT





# Product Flow



Hardware Inputs 

1. Touch
2. Human presence - RCWL-0516



Config

1. User can select if he wants to see the news or not, and set city preference.



Modes

1. **Weather MODE** - Time, location, Date, Day, Weather Type icons
   1. Rotating text - Temperature, Wind speed
2. **ROBOT Eyes** - using whatever libraries
3. **System Mode** - (on Long press 4 sec ) system state, high priority also shown when device boots, wifi connecting
4. **GIF Viewer&#x20;**- 
5. **Thought MODE&#x20;**- random 
6. Meme page - like a photo 
7. NEWS MODE ( Optional ) - Heading of latest new based on users preference ( city )
8. STOCK MODE ( Optional ) - User can select up to 5 stocks to show. Limitation - needs api key
9. HOROSCOPE (Optional ) - User sets the zodiac first time and we show him daily horoscope
10. AUTO PLAY GAME - Bot will auto play some small game
11. Youtube / Instagram Views  - 
12. Cricket Match Score - 



![](assets/rIS4_8Do4fEeIST3FuB4BcWmFrMkWDmBGTw0v6fKueY=.png)

# 1. INPUT LAYER

# Responsibility

Convert hardware/software signals into events.

***

# INPUT SOURCES

## Physical Inputs

* &#x20;touch 
* &#x20;button 
* &#x20;charging state 
* &#x20;future sensors 

***

## System Inputs

* &#x20;boot complete 
* &#x20;WiFi connected 
* &#x20;OTA started 

***

## Time Inputs

* &#x20;idle timeout 
* &#x20;sleep timeout 
* &#x20;scheduled behavior 

***

## Network Inputs

* &#x20;config updated 
* &#x20;connected bot event 

***

# OUTPUT OF INPUT MODULE

It should ONLY output:

# events

Example:

```
{
 "event": "TOUCH_DETECTED"
}
```

***

# IMPORTANT

Input layer should NEVER:

* &#x20;directly trigger animations 
* &#x20;directly update display 

BAD architecture.

***

# MODULE

```
/input
```

# 2. EVENT SYSTEM

VERY IMPORTANT.

This becomes:

# communication layer between systems.

***

# WHY?

Because:

&#x20;modules should NOT tightly depend on each other.

***

# Example

Touch module emits:

```
TOUCH_DETECTED
```

State machine listens.

NOT:

&#x20;touch module directly calling animation engine.

***

# EVENT BUS EXAMPLE

```
eventBus.emit(TOUCH_DETECTED);
```

***

# POSSIBLE EVENTS

| Event           | Source    |
| --------------- | --------- |
| TOUCH\_DETECTED | input     |
| WIFI\_CONNECTED | network   |
| IDLE\_TIMEOUT   | scheduler |
| BOOT\_COMPLETE  | core      |
| CONFIG\_UPDATED | network   |

***

# OUTPUT

Events are consumed by:

* &#x20;state machine 
* &#x20;animation manager 
* &#x20;RGB controller 

***

# MODULE

```
/core/event_bus
```

***

#

# 3. STATE MACHINE

MOST IMPORTANT SYSTEM.

This decides:

# what the bot CURRENTLY IS.

***

# EXAMPLE STATES

## System States

* &#x20;BOOTING 
* &#x20;PAIRING 
* &#x20;UPDATING 
* &#x20;ERROR 

***

## Emotional States

* &#x20;IDLE 
* &#x20;HAPPY 
* &#x20;SLEEPY 
* &#x20;EXCITED 
* &#x20;ANNOYED 

***

## Transitional States

* &#x20;WAKE\_UP 
* &#x20;FALL\_ASLEEP 

***

# RESPONSIBILITY

Takes:

* &#x20;events 
* &#x20;timers 
* &#x20;context 

Outputs:

# current active state

***

# IMPORTANT

State machine should NEVER render directly.

It ONLY decides:

```
what should currently happen
```

***

# EXAMPLE FLOW

```
Current State: IDLE

TOUCH_DETECTED event arrives

State machine:
IDLE → EXCITED
```

***

# OUTPUT

```
{
 "state": "EXCITED"
}
```

***

# MODULE

```
/states
```

***

#

# 4. MOOD ENGINE

(Optional but VERY powerful)

This creates:

# long-term personality behavior.

***

# WHY?

Without mood:

&#x20;bot feels robotic.

***

# Example

Repeated touch:

* &#x20;happiness increases 

Long inactivity:

* &#x20;sleepiness increases 

***

# INTERNAL VARIABLES

```
{
 "energy": 40,
 "happiness": 75,
 "attention": 20
}
```

***

# Mood engine modifies:

* &#x20;animation choices 
* &#x20;RGB themes 
* &#x20;idle behavior 

***

# OUTPUT

Mood modifiers.

***

# Example

```
{
 "mood_modifier": "sleepy"
}
```

***

# MODULE

```
/mood
```

# 5. ANIMATION RESOLVER

THIS is where product quality lives.

***

# RESPONSIBILITY

Given:

* &#x20;current state 
* &#x20;mood 
* &#x20;priority 
* &#x20;timers 

decide:

# which animation should play

***

# INPUTS

```
{
 "state": "IDLE",
 "mood": "sleepy"
}
```

***

# OUTPUT

```
{
 "animation": "idle_sleepy_blink"
}
```

***

# IMPORTANT

This is NOT the renderer.

This only selects animations.

***

# EXAMPLE RULES

| Condition     | Animation        |
| ------------- | ---------------- |
| IDLE + sleepy | slow blink       |
| IDLE + happy  | bouncing eyes    |
| EXCITED       | sparkle reaction |

***

# MODULE

```
/animations/resolver
```

# 6. ANIMATION ENGINE

This actually:

# plays frames.

***

# RESPONSIBILITY

* &#x20;frame timing 
* &#x20;animation loops 
* &#x20;transitions 
* &#x20;frame interpolation 

***

# INPUT

```
{
 "animation": "happy_bounce"
}
```

***

# OUTPUT

Frame stream.

***

# EXAMPLE

```
frame1
frame2
frame3
```

***

# VERY IMPORTANT

Animation engine should support:

* &#x20;interruptible animations 
* &#x20;looping animations 
* &#x20;transition animations 

***

# Example

```
idle → touch reaction → idle
```

smoothly.

***

# MODULE

```
/animations/player
```

# 7. ASSET SYSTEM

THIS is critical.

***

# RESPONSIBILITY

Load animation assets from flash.

***

# STORAGE STRUCTURE

```
/assets
 /expressions
 /themes
 /frames
```

***

# WHAT IS AN ASSET?

Examples:

* &#x20;PNG frames 
* &#x20;compressed binary frames 
* &#x20;theme config 
* &#x20;RGB profile

# 8. RENDERER

THIS actually draws to screen.

***

# RESPONSIBILITY

* &#x20;frame drawing 
* &#x20;screen updates 
* &#x20;buffering 
* &#x20;display driver calls 

***

# INPUT

Frame bitmap.

***

# OUTPUT

Pixels on display.

***

# IMPORTANT

Renderer should know NOTHING about:

* &#x20;emotions 
* &#x20;moods 
* &#x20;touch 
* &#x20;business logic 

ONLY graphics.

***

# MODULE

```
/display/renderer
```

# 10. SCHEDULER

VERY IMPORTANT.

This creates:

# life-like timing.

***

# RESPONSIBILITY

Triggers:

* &#x20;idle animations 
* &#x20;random blinks 
* &#x20;sleep mode 
* &#x20;periodic behaviors 

***

# EXAMPLES

Every:

* &#x20;6–12 sec → blink 
* &#x20;2 min → random reaction 
* &#x20;10 min inactivity → sleepy 

***

# OUTPUT

Events.

***

# MODULE

```
/core/scheduler
```

# 11. NETWORK MANAGER

Simple initially.

***

# RESPONSIBILITY

* &#x20;WiFi 
* &#x20;sync 
* &#x20;OTA 
* &#x20;config fetch 

***

# IMPORTANT

Network should NEVER control rendering directly.

It only:

&#x20;emits events/config changes.

***

# MODULE

```
/network
```

# 12. CONFIG SYSTEM

Stores:

* &#x20;personality 
* &#x20;themes 
* &#x20;brightness 
* &#x20;behavior settings 

***

# INPUT

Backend/local settings.

***

# OUTPUT

Runtime config values.

***

# MODULE

```
/config
```
