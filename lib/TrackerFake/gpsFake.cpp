
#include "TrackerFake.h"
#include "gpsFake.h"

float gpsClass::lat() { TrackerFakeInstance(gps)->lat(); };

float gpsClass::lng() { TrackerFakeInstance(gps)->lng(); };

float gpsClass::encode(int value) { return TrackerFakeInstance(gps)->encode(value); };

int gpsClass::isUpdated() { TrackerFakeInstance(gps)->isUpdated(); };

gpsClass gps;


