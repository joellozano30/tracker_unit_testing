#include <unity.h>        // Incluye la biblioteca de Unity
#include "mpu.h"

void setUp(void) {
    // set stuff up here
    //ArduinoFakeReset();
}

void tearDown(void) {
    // clean stuff up here
}

void test_gpsGetCoordinates_receive_DataCorrectly(void){


}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_gpsGetCoordinates_receive_DataCorrectly);
    


    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}