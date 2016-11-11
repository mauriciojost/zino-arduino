#ifndef DELAYER_INC
#define DELAYER_INC

#include <Log.h>
#include <Misc.h>
#include <actors/Actor.h>

class Delayer : public Actor {

private:
  Actor *actor;
  int offset;   // offset used to start watering some cycles after the match (useful to avoid having electrical load peaks when activating
                // the pumps)
  bool matched; // tells if there was a match in the past for which there is a count down
  int passTheMatchIn; // tells how long until the match will be notified to the downstream actor

public:
  Delayer(Actor *a, int cowo);

  const char *getName();

  void cycle(bool cronMatches);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // DELAYER_INC
