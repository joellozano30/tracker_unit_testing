#ifndef MAIN_H
#define MAIN_H

//#include "config.h"

#define START_UNIT_TESTING

#ifdef START_UNIT_TESTING
    #include "ArduinoFake.h"
#else
    #include "Arduino.h"
    #include "mpu.h"
    #include "sigfox.h"
    #include "fsm.h"
#endif

#endif