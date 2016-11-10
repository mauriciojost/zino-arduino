#ifndef ACTOR_INC
#define ACTOR_INC

class Actor {

public:
  /**
  * Retrieve the name of the actor.
  */
  virtual const char *getName() = 0;

  /**
  * Make the actor execute a cycle.
  */
  virtual void cycle(bool cronMatches) = 0;

  /**
  * Get the current actuator value.
  */
  virtual int getActuatorValue() = 0;

  /**
  * Get the number of configuration states of the actor.
  */
  virtual int getNroConfigs() = 0;

  /**
  * Retrieve configuration message given its index, and set the configuration if requested.
  */
  virtual void setConfig(int configIndex, char *retroMsg, bool set) = 0;

  /**
  * Get the number of info states of the actor.
  */
  virtual int getNroInfos() = 0; // get the number of informations
  virtual void getInfo(int infoIndex, char *retroMsg) = 0;
};

#endif // ACTOR_INC
