#ifndef HUMIDITY_INC
#define HUMIDITY_INC

#include <Log.h>
#include <Misc.h>
#include <actors/Actor.h>

/**
* This actor triggers a match in a children actor when an ADC goes beyond a threshold...
*/
class Humidity : public Actor {

private:
  Actor *actor; // wrapped actor

  int offset;


  bool matched;
  int passTheMatchIn;

public:
  Humidity(Actor *a, int cowo);

  const char *getName();

  void cycle(bool cronMatches);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // HUMIDITY_INC
