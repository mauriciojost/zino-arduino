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

- mode button: to change between modes (either _red_, or with an arrow icon, or on the top)
- set button: to change configuration values in a given mode (either _yellow_, or with an ± icon, or on the bottom)
- reset button: to reset the module (located on the Arduino board itself)

## Modes

### WELCOME MODE 

Upon startup, this mode welcomes the user and tells the version of the module.

### HELP MODE 

This mode tells basic button usage.

### CONFIGURATION MODE 

This mode allows to set up the module and its components. 

- Clock: to set up hour, minutes, and seconds/cycle factor
  - Hours: set the hours
  - Minutes: set the minutes
  - Factor: (+) increase the seconds/cycle factor to calibrate internal timing
  - Factor: (-) decrease the seconds/cycle factor
- Pump A: to set up amount of water per shot
  - Amount: amount of water to be dropped per shot, measured in seconds of pump active
  - Test: if chosen, will activate the pump as soon as in RUN mode letting the user know how much water the system would drop in this position
- Pump B: same as for Pump A
- Pump C: same as for Pump A
- Pump D: same as for Pump A
- Water Level: to set up the water level alarm
  - Minimum: set up the minimum acceptable level of water before triggering the alarm (if set to 0 alarm is deactivated, if set to 1 alarm will be triggered every time water goes below the level sensor.
  
### FREQUENCIES MODE 

This mode allows to set up how often each actor will perform their corresponding duties:

  - Pump A: how often the pump will get triggered
  - Pump B: same as above
  - Pump C: same as above
  - Pump D: same as above
  - Water Level: how often the measure of water level will be triggered
  
### RUN MODE

This mode will put the watering system to work.

