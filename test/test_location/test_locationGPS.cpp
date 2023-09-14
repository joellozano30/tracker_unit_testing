#include <unity.h>        // Incluye la biblioteca de Unity

//#include <Arduino.h>



#include "gps.h"



//#include "mock_integer.h"
//#include 
//#include "gpsTest.h"
//#include "gps.h"

void setUp(void) {
    // set stuff up here

}

void tearDown(void) {
    // clean stuff up here
}

// void test_locationSendViaGPS_should_SaveTheLastCorrectDataforGPS(){

//     float lat,lng;

//     //Data readed by GPS: Satellite Detected
//     lat = -72.12;
//     lng = -12.34;

//     bool eval1 = locationSendViaGPS(true,lat,lng);
//     TEST_ASSERT_EQUAL(lastLatdata,lat);
//     TEST_ASSERT_EQUAL(lastLngdata,lng);

//     //Data readed by GPS: Satellite Undetected
//     lat = 0;
//     lng = 0;

//     bool eval2 = locationSendViaGPS(true,lat,lng);
//     TEST_ASSERT_NOT_EQUAL(lastLatdata,lat);
//     TEST_ASSERT_NOT_EQUAL(lastLngdata,lng);
//     //TEST_ASSERT_EQUAL(lastLngdata,lng);

// }

void test_if_gpsGetCoordinates_receive_data_correctly(void){

    float lat, lng;

    bool valid;

    When(Method(ArduinoFake(Serial), available)).Return(1,1,0);
    When(Method(ArduinoFake(Serial), read)).Return(100);

    valid = gpsGetCoordinates(&lat, &lng);

    TEST_ASSERT_MESSAGE(valid,"No se recibe informacion del GPS por Serial");
}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_if_gpsGetCoordinates_receive_data_correctly);

    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}