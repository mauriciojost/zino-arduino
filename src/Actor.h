#ifndef ACTOR_INC
#define ACTOR_INC

class Actor {

public:

  virtual int getActorState() = 0;
  virtual const char *getActorName() = 0;
  virtual int getNroConfigs() = 0;
  virtual void cycle(bool mustActNow) = 0;
  virtual void setConfig(int configIndex, char *retroMsg, bool set) = 0;
};

#endif // ACTOR_INC
