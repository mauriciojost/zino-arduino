#ifndef CONFIGURABLE_INC
#define CONFIGURABLE_INC

/**
* This class specifies the interface of configurables, components of the
* embedded system which:
* - can be configured via configuration states
* - can inform their state, via info states

These must be inherited by actors to be configured and inform their status.

*/
class Configurable {

public:
  /**
  * Retrieve the name of the configurable.
  */
  virtual const char *getName() = 0;

  /**
  * Get the number of configuration states of the configurable.
  */
  virtual int getNroConfigs() = 0;

  /**
  * Retrieve configuration message given its index, and set the configuration if requested.
  */
  virtual void setConfig(int configIndex, char *retroMsg, bool set) = 0;

  /**
  * Get the number of info states of the configurable.
  */
  virtual int getNroInfos() = 0;

  /**
  * Get an information status of the configurable.
  */
  virtual void getInfo(int infoIndex, char *retroMsg) = 0;
};

#endif // CONFIGURABLE_INC
