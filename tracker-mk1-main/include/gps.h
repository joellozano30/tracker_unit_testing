#ifndef GPS_H
#define GPS_H

#include "Arduino.h"
// #include "TinyGPS.h"
#include "TinyGPS++.h"
// #include "SoftwareSerial.h"

typedef struct{
    float lat;
    float lng;
}gpsCoordinates;

/*#define GPS_RX D5
#define GPS_TX D0*/

#define GPS_RX 16
#define GPS_TX 17

#define GPS_BAUDRATE 9600

void gpsInit(void);
bool gpsGetCoordinates(float *lat, float *lng);
bool gpsCheckLastCoordinates(float lat, float lng);

#endif