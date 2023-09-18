
#include "TrackerFake.h"
#include "gpsFake.h"

double gpsClass::lat() { TrackerFakeInstance(gps)->lat(); };

double gpsClass::lng() { TrackerFakeInstance(gps)->lng(); };

float gpsClass::encode(int value) { return TrackerFakeInstance(gps)->encode(value); };

int gpsClass::isUpdated() { TrackerFakeInstance(gps)->isUpdated(); };

gpsClass gps;


