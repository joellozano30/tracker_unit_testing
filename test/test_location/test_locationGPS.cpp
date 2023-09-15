#include <unity.h>        // Incluye la biblioteca de Unity
#include "gps.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_gpsGetCoordinates_receive_DataCorrectly_by_SerialPort(void){

    float lat, lng;
    bool CoordwereReceived;

    When(Method(ArduinoFake(Serial), available)).Return(1,1,1,0);
    When(Method(ArduinoFake(Serial), read)).Return(100,200);

    CoordwereReceived = gpsGetCoordinates(&lat, &lng);

    TEST_ASSERT_MESSAGE(CoordwereReceived,"No se recibe informacion del GPS por Serial");
}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_gpsGetCoordinates_receive_DataCorrectly_by_SerialPort);

    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}