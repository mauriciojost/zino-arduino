# ZINO USER GUIDE

## Features

These are the features of the module:

- 1 LCD display and 3 buttons for user interaction
- control over 4 independent pump positions / plants (controlled by a rotating servo) fed with an unique pump and a water reservoir
- low level water reservoir alarm
- configurable amount of water dropped per shot per pump position
- configurable frequency of watering per pump position
- 5V power source compatible with USB standards

## Buttons

The module has 3 buttons for user interaction: 

### Mode button

This button allows to change between modes (_configuration mode_ to _run mode_ for instance).

The button is either _red_, or with an arrow icon, or on the top of a pair of buttons.

### Set button

This button allows to change configuration values in a given mode. It works mostly by rolling configuration values (for instance if setting _minutes_, after reaching the value 59 the next value will be 0).

The button is either _yellow_, or with an ± icon, or on the bottom of a pair of buttons.

### Reset button

This button allows to reset the module. It is located on the Arduino board itself.

## Modes

### WELCOME MODE 

Upon startup, this mode welcomes the user and tells the version of the module.

### HELP MODE 

This mode tells basic button usage.

### CONFIGURATION MODE 

This mode allows to set up the module and its components. 

- Clock: to set up hour, minutes, and seconds/cycle factor.
  - Hours: set the hours.
  - Minutes: set the minutes.
  - Factor: (+) increase the seconds/cycle factor to calibrate internal timing (there should be no need to set it up multiple times but only once).
  - Factor: (-) decrease the seconds/cycle factor (it decreases by a delta much smaller than the increase delta, easing factor tuning).
- Plant A: to set up amount of water per shot.
  - Amount: represents the amount of water to be dropped per shot at this plants position, measured in seconds of pump active (can be force-triggered, see below).
  - Test: if chosen, will force the triggering of the watering at a specified position as soon as in RUN mode. This helps letting the user know how much water the system would drop in a given position, and what that position is.
- Plant B: same as for Plant A.
- Plant C: same as for Plant A.
- Plant D: same as for Plant A.
- Water Level: to set up the water level alarm.
  - Minimum: set up the minimum acceptable level of water before triggering the alarm (if set to 0 alarm is deactivated, if set to 1 alarm will be triggered every time water goes below the level sensor.
  
### FREQUENCIES MODE 

This mode allows to set up how often each actor will perform their corresponding duties:

  - Plant A: how often the pump will get triggered
  - Plant B: same as above
  - Plant C: same as above
  - Plant D: same as above
  - Water Level: how often the measure of water level will be triggered
  
The frequency that can be selected for each item above is one among these: 

- once per month
- twice per month
- once per week
- twice per week
- three times per week
- once per day
- twice per day
- once per hour
- twice per hour
- once every 5 minutes
- once every 2 minutes
  
### RUN MODE

This mode will put the watering system to work.

Information modes can be selected using the _set_ button.

