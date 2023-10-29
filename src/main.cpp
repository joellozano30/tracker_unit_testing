#include "main.h"

#ifndef TEST

#define PIN_TO_INTERRUPT 35
uint8_t executeLoop = 0;

void IRAM_ATTR isr() {
  executeLoop = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting test code");
  mpuInit();
  sigfoxInit();
  delay(500);
  Serial.println("Attempting to read modem's information");
  sigfoxReadInfo();
  #ifdef USE_GPS
    gpsInit();
  #endif
  fsmInit();

  // Configuracion pin de interrupcion 
  pinMode(PIN_TO_INTERRUPT, INPUT);
  attachInterrupt(PIN_TO_INTERRUPT, isr, RISING);

  // Se pone a dormir el ESP32
  // esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, HIGH);
  // Serial.println("Going to sleep now");
  // delay(1000);
  // esp_light_sleep_start();
}

void loop() {
  // put your main code here, to run repeatedly:
  //fsmHandler();
  //locationSendViaGps();

  if(executeLoop){
    //Serial.println("Se ha movido el MPU6050");
    fsmHandler();
    executeLoop = false;
  }
}

#else

int main(void)
{
    // initialize LED digital pin as an output.
    //Serial.begin(115200);
    //Serial.println("Starting test code");

    gpsInit();
}

// void loop()
// {
//     // turn the LED on (HIGH is the voltage level)
//     digitalWrite(LED_BUILTIN, HIGH);
//     // wait for a second
//     delay(100);
//     // turn the LED off by making the voltage LOW
//     digitalWrite(LED_BUILTIN, LOW);
//      // wait for a second
//     delay(100);
// }


#endif