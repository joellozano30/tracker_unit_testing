#include <unity.h>        // Incluye la biblioteca de Unity
#include "mpu.h"

void setUp(void) {
    // set stuff up here
    ArduinoFakeReset();
}

void tearDown(void) {
    // clean stuff up here
}

void test_mpuLocationChanged_should_DetectMovement(void){
    mpuStructData *mpuMeasurements;
    int outputLocationFunc = false;

    int flag_set_ubication = 1;
    int flag_evaluate_movement = 1;
    int device_stops_moving = 0;
    int flag_evaluation = 0;
    float var_acel = 0;
    float Axi = 0;
    float Ayi = 0;
    float Azi = 0;

    mpuMeasurements->modulo = 0.3;

    //Se detecta movimiento
    if (mpuMeasurements->modulo > MPU6050_MODULO_VECTOR_LOWER_LIMIT)
    {   

        mpuMeasurements->Ax = 0.44;
        mpuMeasurements->Ay = -1.01;
        mpuMeasurements->Az = 0.10;
        mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));

        if(fabs((mpuMeasurements->modulo) - MPU6050_MODULO_VECTOR_LOWER_LIMIT_STATIC) <= 0.1){
            //Equipo dejo de moverse
            flag_set_ubication = 1;
            flag_evaluate_movement = 1;

            device_stops_moving = 1;
            outputLocationFunc = false;
        }

        TEST_ASSERT_EQUAL_MESSAGE(0, device_stops_moving, "Equipo deja de moverse");

        //Calculo variación de aceleración
        var_acel = 0.4;
        if((var_acel > MPU6050_ACELERATION_VARIATION_LIMIT)) //Detecta sobre aceleracion
            flag_evaluation = 1;

    }
    else{
        flag_set_ubication = 1;
        outputLocationFunc = false;
    }


    if(flag_evaluation)
    {   
        /* False Alarm */
        outputLocationFunc = false;
    }
    else
    {
        /* Movement detected */
        outputLocationFunc = true;
    }

    TEST_ASSERT_EQUAL_MESSAGE(true, outputLocationFunc, "Movement not detected correctly");
}

void test_mpuLocationChanged_should_DetectVibration(void){
    mpuStructData *mpuMeasurements;
    int outputLocationFunc = false;

    int flag_set_ubication = 1;
    int flag_evaluate_movement = 1;
    int device_stops_moving = 0;
    int flag_evaluation = 0;
    float var_acel = 0;
    float Axi = 0;
    float Ayi = 0;
    float Azi = 0;

    mpuMeasurements->modulo = 0.3;

    //Se detecta movimiento
    if (mpuMeasurements->modulo > MPU6050_MODULO_VECTOR_LOWER_LIMIT)
    {   

        mpuMeasurements->Ax = 0.44;
        mpuMeasurements->Ay = -1.01;
        mpuMeasurements->Az = 0.10;
        mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));

        if(fabs((mpuMeasurements->modulo) - MPU6050_MODULO_VECTOR_LOWER_LIMIT_STATIC) <= 0.1){
            //Equipo dejo de moverse
            flag_set_ubication = 1;
            flag_evaluate_movement = 1;

            device_stops_moving = 1;
            outputLocationFunc = false;
        }

        TEST_ASSERT_EQUAL_MESSAGE(0, device_stops_moving, "Equipo deja de moverse");

        //Calculo variación de aceleración
        var_acel = 1;
        if((var_acel > MPU6050_ACELERATION_VARIATION_LIMIT)) //Detecta sobre aceleracion
            flag_evaluation = 1;

    }
    else{
        flag_set_ubication = 1;
        outputLocationFunc = false;
    }


    if(flag_evaluation)
    {   
        /* False Alarm */
        outputLocationFunc = false;
    }
    else
    {
        /* Movement detected */
        outputLocationFunc = true;
    }

    TEST_ASSERT_EQUAL_MESSAGE(false, outputLocationFunc, "Vibration not detected correctly");

}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_mpuLocationChanged_should_DetectMovement);
    RUN_TEST(test_mpuLocationChanged_should_DetectVibration);
    
    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}