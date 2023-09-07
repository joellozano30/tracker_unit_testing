#ifndef GPSTEST_H
#define GPSTEST_H

extern float lastLatdata, lastLngdata;

bool locationSendViaGPS(bool GetCoordinates, float lat, float lng);

#endif