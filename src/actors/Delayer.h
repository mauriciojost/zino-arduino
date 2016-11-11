#ifndef DELAYER_INC
#define DELAYER_INC

#include <Log.h>
#include <Misc.h>
#include <actors/Actor.h>

class Delayer : public Actor {

private:
  Actor *actor;
  int cowOffset; // offset used to start watering some cycles after the match (useful to avoid having electrical load peaks when activating
                 // the pumps)
  bool shouldHaveWatered;   // tells if the watering should have been triggered (but was not due to the offset imposed)
  int shouldHaveWateredAgo; // tells how long ago (in cycles) the watering should have been triggered if offset was zero

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
