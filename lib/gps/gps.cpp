#include "gps.h"

// SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

gpsCoordinates lastCoordinates;

#ifdef TEST
    struct gpsInterface {
    virtual float lat() = 0;
    virtual float lng() = 0;
    };

    Mock<gpsInterface> gps;
#else 
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

bool gpsGetCoordinates(float *lat, float *lng)
{
    #ifndef TEST
        if(!Serial2.available())
    #else
        // Idea is to manipulate in the test -> When(Method(ArduinoFake(Serial), available)).Return(0);
        if(!Serial.available()) //Serial2 ---- Manipular la salida del programa
    #endif
            return false;
    

    #ifndef TEST
        while(Serial2.available())
    #else
        while(Serial.available()) //Serial2 ---- Manipular la salida del programa
    #endif
    {


    #ifndef TEST
        int c = Serial2.read();
    #else
        int c = Serial.read();
    #endif

        //gps.encode(c); //SW
      
        int isUpdated = 1;

//      if(gps.location.isUpdated())  

        if(isUpdated) //SW
        {
            double tempLat, tempLng;

            //tempLat = gps.location.lat();
            //When(Method(gps,lat)).Return(72);
            //tempLng = gps.location.lng();

            tempLat = -72;
            tempLng = -12;

            // Serial.print("[!!] Values read from GPS -> lat: ");
            // //Serial.print(tempLat,8);
            // Serial.print(tempLat);
            // Serial.print(" lng: ");
            // //Serial.println(tempLng,8);
            // Serial.println(tempLng);

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