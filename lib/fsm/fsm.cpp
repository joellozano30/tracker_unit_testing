#include "fsm.h"

mpuStructData mpuReadings;
bool alertAlreadySent;

uint32_t idleStartTime;
uint32_t idleStartWindowTime;

state currentState;
state nextState;

float new_Latdata = 0;
float new_LngData = 0;
float last_Latdata = 0;
float last_Lngdata = 0;
bool response_evaluation = false;

trackerState FSM[NUM_STATES] = {
    {&fsmIdleState},
    {&fsmLocationState}
};

// ****************************************************************
// Private functions
// ****************************************************************

void _idleStateRestartTime(void)
{
    currentState = IDLE_STATE;
    nextState = IDLE_STATE;
    idleStartTime = millis();
    idleStartWindowTime = millis();
}

bool _timeWindowComplete(uint32_t startTime, uint32_t period)
{
    if((millis() - startTime) >= period)
        return true;
    else
        return false;
}

// ****************************************************************
// Public functions
// ****************************************************************

void fsmInit(void)
{
    alertAlreadySent = false;
    idleStartTime = millis();
    idleStartWindowTime = millis();
}

void fsmHandler(void)
{
    nextState = FSM[currentState].stateHandler();
    currentState = nextState;
}

state fsmIdleState(void)
{
    if(mpuLocationChanged(&mpuReadings))
    {    

        set_flag_to_send(ENABLE);

        #ifdef FSM_DEBUG
            Serial.println("[*] Changing to location state. Before changing state we evaluate how far the device has moved to send current location.");
        #endif

        #ifdef USE_GPS

            gpsGetCoordinates(&new_Latdata, &new_LngData);

            if(new_Latdata == 0 && new_LngData == 0){
                // If GPS lost connection and receive 0 in coordinates, sent the last valid values.
                new_Latdata = get_lastLatdata(); 
                new_LngData = get_lastLngdata();
            }

            Serial.print("[*] Actual Coordinates: -> lat: ");Serial.print(new_Latdata);Serial.print(", lng: ");Serial.println(new_LngData);

            last_Latdata = get_lastLatdata();
            last_Lngdata = get_lastLngdata();

            Serial.print("[*] Last Coordinates: -> lat: ");Serial.print(last_Latdata);Serial.print(", lng: ");Serial.println(last_Lngdata);

            response_evaluation = evaluate_distances_between_locations(new_Latdata, new_LngData, last_Latdata, last_Lngdata);

            if(response_evaluation == false){
                if(!locationSendViaGps()){
                    sigfoxEnterSleepMode();
                    return IDLE_STATE;
                }

                sigfoxEnterSleepMode();

                _idleStateRestartTime();

                Serial.println("[!] Flag to send enabled, waiting time to resend ...");
                
                return LOCATION_STATE;
            }
            else{
                Serial.println("[!] The device hasn't moved the minimum distance to start to send location.");
                Serial.println("[*] Keeping in idle state.");
                return IDLE_STATE;
            }
        #else
            locationSendViaWifi();
        #endif

        
    }
    else
    {
        if((millis() - idleStartWindowTime) >= _1_HOUR){

            Serial.println("[!] The time window in idle state has finished.");
            Serial.println("[*] Sending the current location.");

            if(!locationSendViaGps()){
                sigfoxEnterSleepMode();
                return IDLE_STATE;
            }

            sigfoxEnterSleepMode();

            // Restarting Init time ...
            _idleStateRestartTime();

            Serial.println("[*] Keeping in idle state.");
        }

        return IDLE_STATE;

    }
}

state fsmLocationState(void)
{
    // Polling on this state should be constant
    // Messages will be sent every 5 minutes as soon as movement on the MPU is detected.

    if(!_timeWindowComplete(idleStartTime, _5_MINUTES ))
    {
        #ifdef FSM_DEBUG
        // Serial.println("Still waiting for 2 minute window to pass");
        #endif
        return LOCATION_STATE;
    }
    // else if(!mpuLocationChanged(&mpuReadings))
    else if(get_flag_to_send())
    {
        gpsGetCoordinates(&new_Latdata, &new_LngData);

        Serial.print("[*] Actual Coordinates: -> lat: ");Serial.print(new_Latdata);Serial.print(", lng: ");Serial.println(new_LngData);

        last_Latdata = get_lastLatdata();
        last_Lngdata = get_lastLngdata();

        Serial.print("[*] Last Coordinates: -> lat: ");Serial.print(last_Latdata);Serial.print(", lng: ");Serial.println(last_Lngdata);

        response_evaluation = evaluate_distances_between_locations(new_Latdata, new_LngData, last_Latdata, last_Lngdata);

        _idleStateRestartTime();

        if(response_evaluation == false){ // SET TO FALSE
            
            Serial.println("Keep sending coordinates every 5 minutes");
            locationSendViaGps();

            return LOCATION_STATE;
        }
        else{

            set_flag_to_send(DISABLE);  

            Serial.println("Device was not moved from the last position. Changing state: Location -> Idle");

            return IDLE_STATE;

            // #ifdef FSM_DEBUG
            //     Serial.println("Tracker stopped. Changing state: Location -> Idle");
            // #endif
        }
    }
    else
    {
        #ifdef USE_GPS
            locationSendViaGps();
        #else
            //locationSendViaWifi();
        #endif
        _idleStateRestartTime();
        return LOCATION_STATE;
    }
}