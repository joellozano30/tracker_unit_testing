#include "main.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting test code");
  mpuInit();
  sigfoxInit();
  delay(500);
  Serial.println("Attempting to read modem's information");
  sigfoxReadInfo();
  // hola
  #ifdef USE_GPS
    gpsInit();
  #endif

  fsmInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  fsmHandler();
  //locationSendViaGps();
}