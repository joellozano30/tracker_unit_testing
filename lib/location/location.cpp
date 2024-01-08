#include "location.h"
#include "mpu.h"


// structMacPair latestSavedMacs;
float lastLatdata = 0, lastLngdata = 0;
int flag_start_to_send = false;

bool locationSendViaGps(void)
{
    String geolocationMessage = "";
    float lat, lng, temperature;
    if(gpsGetCoordinates(&lat, &lng))
    {
        if((lat == 0 && lng == 0)){
            Serial.println("Sending last valid values. If coordinates are 0 is because the device hasn't detected valid values yet.");
            lat = lastLatdata;
            lng = lastLngdata;
        }
        // Start Sigfox Communication
        sigfoxSendWakeUp();
        temperature = getTemperatureData();
        Serial.print("[*] Coordinates received from GPS and MPU -> lat: "); Serial.print(lat); Serial.print(", lng: "); Serial.print(lng); Serial.print(", T: "); Serial.println(temperature);
        sigfoxPackGPSMsg(lat, lng, temperature, &geolocationMessage);
        Serial.print("[!] Sigfox message to send: "); Serial.println(geolocationMessage);
        #ifndef NO_SEND_MSG
        sigfoxSendMsg(geolocationMessage); // Envio de mensaje
        #endif
        set_recalibrate(true);

        //Guardamos últimos datos válidos:
        if(lat != 0 && lng != 0){
            Serial.println("Saving last valid values.");
            lastLatdata = lat;
            lastLngdata = lng;
        }
        return true;
    }
    else
    {
        Serial.println("[!] Something happened when attempting to retrieve the corrdinates");
        return false;
    }
}

void set_flag_to_send(uint8_t state){
    flag_start_to_send = state;
}

int get_flag_to_send(void){
    return flag_start_to_send;
}

float get_lastLatdata(void){
    return lastLatdata;
}

float get_lastLngdata(void){
    return lastLngdata;
}

bool evaluate_distances_between_locations(float lat1, float lng1, float lat2, float lng2){
    float distance = 0;
    distance = (2*EARTH_RADIUS*asin(sqrt(pow(sin(((lat2-lat1)*(PI/180))/2),2) + cos(lat1*(PI/180)) * cos(lat2*(PI/180)) * pow(sin(((lng2-lng1)*(PI/180))/2),2))))*1000; //in km
    
    #ifndef TEST
    Serial.print("[*] The distance between the two coordinates is: ");
    Serial.println(distance);
    #endif

    if(distance <= LIMIT_MINIMUM_RADIUS_M)
        return true;
    else 
        return false;
}

// void locationSendViaWifi(void)
// {
//     String geolocationMessage = "";
//     uint8_t mac1[6] = {0};
//     uint8_t mac2[6] = {0};

//     wifiStartScan(6000, 4, true);

//     if(wifiGetFirstAndSecondBestAPs(mac1, mac2) == 2)
//     {
//         #ifdef WIFI_LOCATION_DEBUG
//             Serial.println("Obtained MAC addresses. Verifying if they are not the same ones as last message");
//             Serial.println("Previous MAC addresses: ");
//             Serial.print("MAC1: ");
//             Print_mac(latestSavedMacs.mac1);
//             Serial.print("MAC2: ");
//             Print_mac(latestSavedMacs.mac2);
//             Serial.println("Current MAC address: ");
//             Serial.print("MAC1: ");
//             Print_mac(mac1);
//             Serial.print("MAC2: ");
//             Print_mac(mac2);
//         #endif

//         if(!wifiCompareMacAddresses(mac1, mac2, &latestSavedMacs))
//         {
//             #ifdef WIFI_LOCATION_DEBUG
//                 Serial.println("Sending Geolocation message!");
//             #endif
//             sigfoxPackGeolocationMessage(mac1, mac2, &geolocationMessage);
//             sigfoxSendMsg(geolocationMessage);
//         }
//         else
//         {
//             #ifdef WIFI_LOCATION_DEBUG
//                 Serial.println("Repeated MAC addresses. Tracker stopped. Not sending any message.");
//             #endif
//         }
//         // Storing current MAC addresses
//         memcpy(latestSavedMacs.mac1, mac1, 6);
//         memcpy(latestSavedMacs.mac2, mac2, 6);
//     }
// }