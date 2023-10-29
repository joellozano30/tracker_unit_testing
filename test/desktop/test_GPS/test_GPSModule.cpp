#include <unity.h>        // Incluye la biblioteca de Unity
#include "gps.h"

void setUp(void) {
    // set stuff up here
    ArduinoFakeReset();
}

void tearDown(void) {
    // clean stuff up here
}

void test_gpsGetCoordinates_shoulReceive_DataCorrectly(void){

    float lat, lng;
    bool CoordwereReceived;

    //Arduino Functions
    When(Method(ArduinoFake(Serial), available)).Return(1,1,0);
    When(Method(ArduinoFake(Serial), read)).Return(100);

    //GPS functions
    Fake(Method(TrackerFake(gps), encode));
    When(Method(TrackerFake(gps), isUpdated)).Return(1);
    When(Method(TrackerFake(gps), lat)).Return(-77); 
    When(Method(TrackerFake(gps), lng)).Return(-12);

    CoordwereReceived = gpsGetCoordinates(&lat, &lng);

    TEST_ASSERT_MESSAGE(CoordwereReceived,"No se recibe informacion del GPS por Serial");
}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_gpsGetCoordinates_shoulReceive_DataCorrectly);
    
    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}