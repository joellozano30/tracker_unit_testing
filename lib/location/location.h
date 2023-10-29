#ifndef LOCATION_H
#define LOCATION_H

#ifndef TEST
    #include "Arduino.h"
#else
    #include "ArduinoFake.h"
#endif

#include "gps.h"
#include "config.h"
#include "sigfox.h"

bool locationSendViaGps(void);
//void locationSendViaWifi(void);

#endif