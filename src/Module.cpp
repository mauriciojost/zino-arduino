#include <Module.h>

#define PUMP_ACTIVATION_OFFSET_UNIT 60

Module::Module() {

  this->amountOfActors = 3;
  this->actors = new Actor*[amountOfActors];

//ry = new int*[rowCount];

  this->p0 = new Pump(MSG_PUMP_NAME0);
  this->pump0 = new Delayer(p0, PUMP_ACTIVATION_OFFSET_UNIT * 0);
  this->actors[0] = pump0;

  this->p1 = new Pump(MSG_PUMP_NAME1);
  this->pump1 = new Delayer(p1, PUMP_ACTIVATION_OFFSET_UNIT * 1);
  this->actors[1] = pump1;

  this->level = new Level(MSG_LEVEL_NAME);
  this->actors[2] = level;

  this->clock = new Clock(amountOfActors);

  this->amountOfConfigurables = amountOfActors + 1;
  this->configurables = new Configurable*[amountOfConfigurables];
  this->configurables[0] = clock;
  this->configurables[1] = pump0;
  this->configurables[2] = pump1;
  this->configurables[3] = level;

  this->bot = new Bot(clock, actors, amountOfActors, configurables, amountOfConfigurables);

  this->lcd = new Lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

}