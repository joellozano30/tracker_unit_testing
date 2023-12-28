#include <unity.h>        // Incluye la biblioteca de Unity
#include "gps.h"
#include "location.h"

void setUp(void) {
    // set stuff up here
    ArduinoFakeReset();
}

void tearDown(void) {
    // clean stuff up here
}

void test_evaluation_between_two_distances(void){

    float lat1, lng1, lat2, lng2;
    float evaluation;

    lat1 = -12.11817741394043;
    lng1 = -76.98937225341797;
    lat2 = -12.118196487426758;
    lng2 = -76.9893569946289;

    evaluation = evaluate_distances_between_locations(lat1, lng1, lat2, lng2);

    TEST_ASSERT_MESSAGE(evaluation,"Distance is not lower than the limit");
}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_evaluation_between_two_distances);
    
    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}