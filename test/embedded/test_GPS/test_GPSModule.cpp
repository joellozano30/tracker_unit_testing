#include <unity.h>        // Incluye la biblioteca de Unity
#include "gps.h"
#include "mpu.h"

void setUp(void) {
    // set stuff up here
  Serial.begin(115200);
  Serial.println("Starting test code");
  mpuInit();
  //sigfoxInit();
  delay(500);
  Serial.println("Attempting to read modem's information");
  //sigfoxReadInfo();
  #ifdef USE_GPS
    gpsInit();
  #endif
  //fsmInit();
}

void tearDown(void) {
    // clean stuff up here
}

void test_gpsGetCoordinates_shoulReceive_DataCorrectly(void){

    float lat = 0, lng = 0;
    bool CoordwereReceived = false;
    bool latDifferentZero;
    bool lngDifferentZero;

    CoordwereReceived = gpsGetCoordinates(&lat, &lng);

    latDifferentZero = lat!= 0;
    lngDifferentZero = lng!= 0;

    TEST_ASSERT_MESSAGE(CoordwereReceived,"No se recibe informacion del GPS por Serial");

    TEST_ASSERT_EQUAL_MESSAGE(true,latDifferentZero,"Latitude equal 0");
    TEST_ASSERT_EQUAL_MESSAGE(true,lngDifferentZero,"Longitude equal 0");
}

void setup() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_gpsGetCoordinates_shoulReceive_DataCorrectly);
    
    UNITY_END(); // Finaliza Unity Test Framework
}

void loop(){
    // Executes Nothing
}