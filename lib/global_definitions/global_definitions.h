#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

typedef enum alert_type{
    POWER_OFF = 0,
    MOVEMENT_DETECTED,
    LOCATION_CHANGED
}alert_type;

#define NUMBER_OF_SAMPLES               60
#define TIME_TO_WAIT                    60000
#define MAX_DAILY_SIGFOX_MESSAGES       140
#define _15_SECONDS                     15000UL
#define _2_MINUTES                      120000UL
#define _5_MINUTES                      300000UL
#define _20_MINUTES                     1200000UL 
#define _1_HOUR                         3600000UL 
#define WIFI_TIMEOUT                    10000UL

#endif