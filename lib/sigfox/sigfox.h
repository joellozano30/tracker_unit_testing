#ifndef SIGFOX_H
#define SIGFOX_H

#include "Arduino.h"
// #include "SoftwareSerial.h"
#include "global_definitions.h"
#include "config.h"

#define SIGFOX_RX D7
#define SIGFOX_TX D8

#define SIGFOX_ENABLE 13

void sigfoxInit(void);
void sigfoxChangeToRegion4(void);
void sigfoxReadInfo(void);
void sigfoxSendATCommand(char* comandoAT);
void sigfoxSendAlert(alert_type sigfox_alert);
void sigfoxPackGeolocationMessage(uint8_t *mac1, uint8_t *mac2, String *geoloc_message);
void sigfoxPackGPSMsg(float lat, float lng, float temperature, String *geoloc_message);
void sigfoxSendMsg(String buf_tx);


#endif