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



}


void test_mpuLocationChanged_should_EvaluateMovement_with_AReference(void){

    When(Method(ArduinoFake(), millis)).AlwaysReturn();
    static uint32_t calibration_time = millis();

    mpuStructData *mpuMeasurements;

    float Axi=0;
    float Ayi=0;
    float Azi=0;

    int flag_set_ubication = 1;

    int i=0;

    while(i<1){

        When(Method(TrackerFake(i2c), i2cReadValueArray)).AlwaysReturn(); //Do nothing
        mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
        mpuConvertRawValues(mpuMeasurements);

        When(Method(ArduinoFake(), millis)).AlwaysReturn();
        if(((millis() - calibration_time) >= MPU6050_TIME_TO_CALIBRATE) && flag_set_ubication){
            //Valores nuevos normalizados
            Axi = mpuMeasurements->Ax;
            Ayi = mpuMeasurements->Ay;
            Azi = mpuMeasurements->Az;

            calibration_time = millis();
        }

        // if(Axi == 0 || Ayi == 0 || Azi == 0){
        //     //Valores normalizados iniciales
        //     mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
        //     mpuConvertRawValues(mpuMeasurements);
        //     Axi = mpuMeasurements->Ax;
        //     Ayi = mpuMeasurements->Ay;
        //     Azi = mpuMeasurements->Az;
        // }

        // mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));
        
        // if (mpuMeasurements->modulo > MPU6050_MODULO_VECTOR_LOWER_LIMIT)
        //     flag_set_ubication = 0;
        
        i++;
    }
    TEST_ASSERT_EQUAL(1, Axi);
}


void test_getTemperatureData_receive_DataCorrectly(void){


}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    //RUN_TEST(test_mpuLocationChanged_should_EvaluateMovement_with_AReference);
    RUN_TEST(test_mpuLocationChanged_should_DetectMovement);
    RUN_TEST(test_getTemperatureData_receive_DataCorrectly);
    
    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}