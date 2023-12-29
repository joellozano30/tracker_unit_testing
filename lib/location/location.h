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
#include "math.h"

#define ENABLE 1
#define DISABLE 0
#define EARTH_RADIUS 6371
#define LIMIT_MINIMUM_RADIUS_M 100 

bool locationSendViaGps(void);
void set_flag_to_send(uint8_t state);
int get_flag_to_send(void);
float get_lastLatdata(void);
float get_lastLngdata(void);
bool evaluate_distances_between_locations(float lat1, float lng1, float lat2, float lng2);
//void locationSendViaWifi(void);

#endif