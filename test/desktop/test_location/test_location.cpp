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

void test_evaluation_between_two_distances_should_be_lower_than_the_limit_established(void){

    float lat1, lng1, lat2, lng2;
    float evaluation;

    lat1 = -12.116899490356445;
    lng1 = -76.99018859863281;
    lat2 = -12.11646842956543;
    lng2 = -76.9906005859375;

    evaluation = evaluate_distances_between_locations(lat1, lng1, lat2, lng2);

    TEST_ASSERT_MESSAGE(evaluation,"Distance between two coordinates is not lower than the limit");
}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_evaluation_between_two_distances_should_be_lower_than_the_limit_established);
    
    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}