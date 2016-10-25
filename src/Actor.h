#ifndef ACTOR_INC
#define ACTOR_INC

class Actor {

public:

  virtual int isDriven() = 0;
  virtual const char *getName() = 0;
  virtual int getNroConfigStates() = 0;
  virtual void cycle(bool mustWaterNow) = 0;
  virtual void setConfig(int configState, char *retroMsg, bool set) = 0;
};

#endif // ACTOR_INC
