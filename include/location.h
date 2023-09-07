#ifndef LOCATION_H
#define LOCATION_H

#include "Arduino.h"
#include "gps.h"
#include "wifi.h"
#include "config.h"
#include "sigfox.h"

bool locationSendViaGps(void);
void locationSendViaWifi(void);

#endif