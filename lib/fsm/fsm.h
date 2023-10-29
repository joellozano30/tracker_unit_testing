#ifndef FSM_H
#define FSM_H

#ifndef TEST
    #include "Arduino.h"
#else
    #include "ArduinoFake.h"
#endif

#include "mpu.h"
#include "global_definitions.h"
#include "location.h"

#define USE_GPS

typedef enum {
    IDLE_STATE=0,
    LOCATION_STATE,
    NUM_STATES
}state;

typedef struct{
    state (*stateHandler)(void);
}trackerState;

void fsmInit(void);
void fsmHandler(void);
state fsmIdleState(void);
state fsmLocationState(void);

#endif