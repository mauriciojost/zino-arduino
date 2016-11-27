# ZINO USER GUIDE

## Features

These are the features of the module:

- 1 LCD display and 3 buttons for user interaction
- control over 4 independent pump positions (controlled by a rotating servo) fed with an unique water reservoir
- low level water reservoir alarm
- configurable amount of water dropped per shot per pump position
- configurable frequency of watering per pump position (once per month, twice per month, once per week, twice per week, etc.)
- 5V power source compatible with USB standards

## Buttons

The module has 3 buttons for user interaction: 

- mode button: to change between modes
- set button: to change configuration values in a given mode
- reset button: to reset the module

## Modes

- WELCOME MODE (tells the version of the module)

- HELP MODE (tells basic button usage)

- CONFIGURATION MODE (to set up the module)
  - Clock (to set up hour, minutes, and factor)
    - Hours (set the hours)
    - Minutes (set the minutes)
    - Factor (+) (increase the seconds/cycle factor to calibrate internal timing)
    - Factor (-) (decrease the seconds/cycle factor)
  - Pump A (to set up amount of water per shot)
    - Amount (amount of water to be dropped per shot, measured in seconds of pump active)
    - Test (if chosen, will activate the pump as soon as in RUN mode)
  - Pump B (same as for Pump A)
    - Amount
    - Test
  - Pump C (same as for Pump A)
    - Amount
    - Test
  - Pump D (same as for Pump A)
    - Amount
    - Test
  - Water Level (to set up the water level alarm)
    - Minimum (set up the minimum acceptable before triggering the alarm)
    
- FREQUENCIES MODE (how often the actor will act)
  - Pump A (how often the pump will get triggered)
  - Pump B (same as above)
  - Pump C (same as above)
  - Pump D (same as above)
  - Water Level (how often the measure of water level will be triggered)
  
- RUN MODE (put the watering system to work)

