#ifndef MAIN_H
#define MAIN_H


#ifdef UNIT_TEST
    #include "ArduinoFake.h"
#else
    #include "Arduino.h"
#endif

//#include "Arduino.h"
#include "mpu.h"
#include "sigfox.h"
#include "fsm.h"

#endif