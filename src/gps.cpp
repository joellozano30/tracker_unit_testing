#include "gps.h"

// SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus gps;

gpsCoordinates lastCoordinates;

void gpsInit(void)
{
    Serial2.begin(GPS_BAUDRATE);
    lastCoordinates.lat = 0.0;
    lastCoordinates.lng = 0.0;
}

bool gpsGetCoordinates(float *lat, float *lng)
{
    if(!Serial2.available())
        return false;
    
    while(Serial2.available())
    {
        int c = Serial2.read();
        gps.encode(c);
        // Serial.write(c);
        // Serial.println("");

        if(gps.location.isUpdated())
        {
            double tempLat, tempLng;

            tempLat = gps.location.lat();
            tempLng = gps.location.lng();
            Serial.print("[!!] Values read from GPS -> lat: ");
            Serial.print(tempLat,8);
            Serial.print(" lng: ");
            Serial.println(tempLng,8);

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

    // return gpsCheckLastCoordinates(*lat, *lng);
    return true;
}

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