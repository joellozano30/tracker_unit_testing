#include "main.h"

#ifndef TEST

#define PIN_TO_INTERRUPT 35
#define PIN_BUTTON 15

uint8_t executeLoop = false;

void IRAM_ATTR isr() {
  executeLoop = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("[*] Hi my friend");
  mpuInit();
  sigfoxInit();
  delay(500);
  Serial.println("[*] Attempting to read modem's information");
  sigfoxReadInfo();
  #ifdef USE_GPS
    gpsInit();
  #endif
  fsmInit();

  // Configuracion pin de interrupcion 
  //pinMode(PIN_TO_INTERRUPT, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  attachInterrupt(PIN_BUTTON, isr, FALLING);

  // Se pone a dormir el ESP32
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, LOW);
  executeLoop = false;
  Serial.println("[*] Going to sleep now");
  delay(1000);
  sigfoxEnterSleepMode();
  esp_light_sleep_start();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(executeLoop){
    //fsmHandler();
    locationSendViaGps();
    sigfoxEnterSleepMode();
    executeLoop = false;
    esp_light_sleep_start();
  }
}

#else

int main(void)
{
  
}

#endif