#include "gpsTest.h"

float lastLatdata = 0 , lastLngdata = 0;

bool locationSendViaGPS(bool GetCoordinates, float lat, float lng){

    if(GetCoordinates){
        
        //Se guardan los últimos datos válidos
        if(!(lat != 0 && lng != 0)){
            lat = lastLatdata;
            lng = lastLngdata;
        }
        
        //Empaquetado de informacion:

        //sigfoxPackGPSMsg
        //sigfoxSendMsg

        //Guardamos últimos datos válidos:
        if(lat != 0 && lng != 0){
            lastLatdata = lat;
            lastLngdata = lng;
        }


        return true;
    }
    else
        return false;
    
    
}