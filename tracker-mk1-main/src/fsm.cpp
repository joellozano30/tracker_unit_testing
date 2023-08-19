#include "fsm.h"

mpuStructData mpuReadings;
bool alertAlreadySent;

uint32_t idleStartTime;

state currentState;
state nextState;

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
        #ifdef FSM_DEBUG
            Serial.println("Changing to location state. Before changing state we send current location.");
        #endif

        #ifdef USE_GPS
            if(!locationSendViaGps())
                return IDLE_STATE;
        #else
            locationSendViaWifi();
        #endif
        _idleStateRestartTime();
        return LOCATION_STATE;
    }
    else
    {
        return IDLE_STATE;
    }
}

state fsmLocationState(void)
{
    // Polling on this state should be constant
    // Messages will be sent every 5 minutes as soon as movement on the MPU is detected.

    if(!_timeWindowComplete(idleStartTime, _15_SECONDS))
    {
        #ifdef FSM_DEBUG
        //    Serial.println("Still waiting for 2 minute window to pass");
        #endif
        return LOCATION_STATE;
    }
    else if(!mpuLocationChanged(&mpuReadings))
    {
        #ifdef FSM_DEBUG
            Serial.println("Tracker stopped. Changing state: Location -> Idle");
        #endif
        _idleStateRestartTime();
        return IDLE_STATE;
    }
    else
    {
        #ifdef USE_GPS
            locationSendViaGps();
        #else
            locationSendViaWifi();
        #endif
        _idleStateRestartTime();
        return LOCATION_STATE;
    }
}