#ifndef GPS_H
#define GPS_H

#ifndef TEST
    #include "Arduino.h"
// #include "TinyGPS.h"
    #include "TinyGPS++.h"
// #include "SoftwareSerial.h"
#else
    #include "ArduinoFake.h"
    #include <math.h>
    #include <stdio.h>
    
    using namespace fakeit;
    #include "TrackerFake.h"
#endif

typedef struct{                
    float lat;
    float lng;
}gpsCoordinates; //SW

/*#define GPS_RX D5
#define GPS_TX D0*/

#define GPS_RX 16
#define GPS_TX 17

#define GPS_BAUDRATE 9600

void gpsInit(void);
bool gpsGetCoordinates(float *lat, float *lng);
bool gpsCheckLastCoordinates(float lat, float lng);


#endif

