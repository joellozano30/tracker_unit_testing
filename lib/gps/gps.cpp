#include "gps.h"

// SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

gpsCoordinates lastCoordinates;

#ifdef TEST
    // ------ Mocking declarations ------
    struct gpsInterface {
        virtual float lat() = 0;
        virtual float lng() = 0;
        virtual float encode(int);
        virtual int isUpdated();
    };

    Mock<gpsInterface> gpsMock;

    gpsInterface & gps = gpsMock.get();
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
            return false;
        while(Serial2.available())
        {
        int c = Serial2.read();
        gps.encode(c);

        if(gps.location.isUpdated()) 
    #else
        if(!Serial.available()) //Serial2 ---- Manipular la salida del programa
            return false;

        while(Serial.available()) //Serial2 ---- Manipular la salida del programa
        {
        int c = Serial.read();

        // ----- Mocking -----

        Fake(Method(gpsMock,encode)); //Do nothing
        gps.encode(c); //Do nothing


        When(Method(gpsMock,isUpdated)).Return(1);
        if(gps.isUpdated())
    #endif

        {
            double tempLat, tempLng;

        #ifndef TEST
            tempLat = gps.location.lat();
            tempLng = gps.location.lng();
        #else
            // ----- Mocking -----

            When(Method(gpsMock,lat)).Return(-77.003618);
            tempLat = gps.lat();

            When(Method(gpsMock,lng)).Return(-12.012173);
            tempLng = gps.lng();
        #endif

            //Serial.print("[!!] Values read from GPS -> lat: ");

        #ifndef TEST
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