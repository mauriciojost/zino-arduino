#ifndef MODULE_INC
#define MODULE_INC

#include <actors/Actor.h>
#include <actors/Configurable.h>
#include <actors/Pump.h>
#include <actors/Delayer.h>
#include <actors/Level.h>
#include <ui/Lcd.h>
#include <Clock.h>
#include <Bot.h>
#include <hardware/AppPins.h>

/**
* This class represents the integration of all components (LCD, buttons, buzzer, etc).
*/
class Module {

private:

public:
  int amountOfActors;
  Pump* p0;
  Delayer* pump0;
  Pump* p1;
  Delayer* pump1;
  Level* level;
  Actor **actors;
  Clock* clock;
  int amountOfConfigurables;
  Configurable **configurables;
  Bot* bot;
  Lcd* lcd;

  Module();
};

#endif // MODULE_INC
