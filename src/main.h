#ifndef MAIN_H
#define MAIN_H


#ifdef TEST
    #include "ArduinoFake.h"
#else
    #include "Arduino.h"
#endif

#include "mpu.h"
#include "sigfox.h"
#include "fsm.h"

#endif