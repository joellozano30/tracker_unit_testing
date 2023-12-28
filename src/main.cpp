#include "main.h"

#ifndef TEST

#define PIN_TO_INTERRUPT 35
uint8_t executeLoop = false;

void IRAM_ATTR isr() {
  executeLoop = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("[*] Hi cholito pantalón blanco");
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
  pinMode(PIN_TO_INTERRUPT, INPUT);
  attachInterrupt(PIN_TO_INTERRUPT, isr, RISING);

  // Se pone a dormir el ESP32
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, HIGH);
  executeLoop = false;
  Serial.println("[*] Going to sleep now");
  delay(1000);
  sigfoxEnterSleepMode();
  esp_light_sleep_start();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(executeLoop){
    fsmHandler();
    executeLoop = false;
  }
}

#else

int main(void)
{
  
}

#endif