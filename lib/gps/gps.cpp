#include "gps.h"

// SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

gpsCoordinates lastCoordinates;

#ifndef TEST
TinyGPSPlus gps;
#endif

void gpsInit(void)
{
    #ifndef TEST
    Serial2.begin(GPS_BAUDRATE); //HW
    #else
    Serial.begin(GPS_BAUDRATE);
    #endif
    lastCoordinates.lat = 0.0;
    lastCoordinates.lng = 0.0;
}

#ifndef TEST 

bool gpsGetCoordinates(float *lat, float *lng){

    if(!Serial2.available())
        return false;

    while(Serial2.available())
    {
        int c = Serial2.read();
        gps.encode(c);

        if(gps.location.isUpdated()){
            double tempLat, tempLng;

            tempLat = gps.location.lat();
            tempLng = gps.location.lng();

        #ifndef TEST
            Serial.print("[!!] Values read from GPS -> lat: ");
            Serial.print(tempLat,8);
            Serial.print(" lng: ");
            Serial.println(tempLng,8);
        #endif
        
            *lat = (float)tempLat;
            *lng = (float)tempLng;
            
            lastCoordinates.lat = *lat;
            lastCoordinates.lng = *lng;
        }
        else
        {
            *lat = lastCoordinates.lat;
            *lng = lastCoordinates.lng;
        }
    }

    return true;
}

#else

bool gpsGetCoordinates(float *lat, float *lng)
{
    if(!Serial.available()) 
        return false;
    while(Serial.available()) 
    {
        int c = Serial.read();
        gps.encode(c); 

        if(gps.isUpdated()){

            double tempLat, tempLng;
            tempLat = gps.lat();
            tempLng = gps.lng();

        #ifndef TEST
            Serial.print("[!!] Values read from GPS -> lat: ");
            Serial.print(tempLat,8);
            Serial.print(" lng: ");
            Serial.println(tempLng,8);
        #endif
            *lat = (float)tempLat;
            *lng = (float)tempLng;
            
            lastCoordinates.lat = *lat;
            lastCoordinates.lng = *lng;
        }
        else
        {
            *lat = lastCoordinates.lat;
            *lng = lastCoordinates.lng;
        }
    }

    return true;
}

#endif

bool gpsCheckLastCoordinates(float lat, float lng)
{
    float latDiff = lastCoordinates.lat - lat;
    float lngDiff = lastCoordinates.lng - lng;

    latDiff = abs(latDiff);
    lngDiff = abs(lngDiff);

    if((latDiff <= 0.001) && (lngDiff <= 0.001))
    {
        Serial.println("[!] Current coordinates match last coordinates");
        return false;
    }
    else
    {
        Serial.print("[+] Current coordinates are different from last coordinates, Last Coordinates -> lat: ");
        Serial.print(lastCoordinates.lat, 5); Serial.print(" lng: "); Serial.print(lastCoordinates.lng, 5);
        Serial.print(", Current Coordinates -> lat: "); Serial.print(lat, 5); Serial.print(" lng: "); Serial.println(lng, 5);
        lastCoordinates.lat = lat;
        lastCoordinates.lng = lng;
        return true;
    }
}