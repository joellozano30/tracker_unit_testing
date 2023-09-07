#ifndef FSM_H
#define FSM_H

#include "Arduino.h"
#include "mpu.h"
#include "location.h"
#include "global_definitions.h"

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