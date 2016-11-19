#ifndef ACTOR_INC
#define ACTOR_INC

#include <actors/Configurable.h>

/**
* This class specifies the interface of actors, components of the
* embedded system which:
* - can be configured via configuration states (via [[Configurable]])
* - can inform their state, via info states (via [[Configurable]])
* - can inform the actuator value (if any) (a LED, a pump, a buzzer)
*/
class Actor : public Configurable {

public:
  
  /**
  * Make the actor execute a cycle.
  */
  virtual void cycle(bool cronMatches) = 0;

  /**
  * Get the current actuator value.
  */
  virtual int getActuatorValue() = 0;

};

#endif // ACTOR_INC
