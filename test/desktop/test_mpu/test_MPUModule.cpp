#include <unity.h>        // Incluye la biblioteca de Unity
#include "mpu.h"
#include "integer.h"

void setUp(void) {
    // set stuff up here
    ArduinoFakeReset();
}

void tearDown(void) {
    // clean stuff up here
}

void test_calculate_array_in_movement_shouldSaveValues(void){
    mpuStructData mpuMeasurements;
    int num_elements=0;

    When(Method(ArduinoFake(), millis)).Return(0,25,50,50,50,100,100,100,200,200,200,300,300,300,400);
    Fake(Method(TrackerFake(i2c), i2cReadValueArray));
    When(OverloadedMethod(ArduinoFake(Serial), print, size_t(const char*))).AlwaysReturn();
    When(OverloadedMethod(ArduinoFake(Serial), println, size_t(const char*))).AlwaysReturn();

    num_elements = get_num_elements_of_acc_module();
    TEST_ASSERT_EQUAL_MESSAGE(0,num_elements,"Elements existing in array");

    calculate_acc_module_array_in_movement(&mpuMeasurements);

    num_elements = get_num_elements_of_acc_module();
    TEST_ASSERT_EQUAL_MESSAGE(5,num_elements,"Elements where not saved in array");
}

void test_f_der_shouldCalculate_VariationCorrectly(void){
    float acc_module_vector[10] = {0.2,0.4,0.0,0.9,1.0,1.1,1.3,1.4,0.5,0.6};
    float var_acel;
    int eval_var_acel=false;

    var_acel = f_der(acc_module_vector, 10);
    eval_var_acel = (var_acel -1.4)<0.000001;

    TEST_ASSERT_EQUAL_MESSAGE(true,eval_var_acel,"Error in calculus");
}

void test_calc_dev_estandar_shouldCalculate_devEstandar_Correctly(void){
    float acc_module_vector[10] = {1,1.5,1.3,0.4,0.8,1.7,0.6,0.5,1.2,1.0};
    float dev_estandar;
    int eval_dev_est = false;

    dev_estandar = calc_dev_estandar(acc_module_vector, 10);
    eval_dev_est = (dev_estandar-0.409878)<0.000001; //Valor de desviacion estandar conocido

    TEST_ASSERT_EQUAL_MESSAGE(true,eval_dev_est,"Error in calculus of dev. estandar");

}


void test_evaluate_movement_or_vibration_shouldDetect_Vibration(void){

    mpuStructData *mpuMeasurements;
    bool response;

    float value = 0.9;

    response = evaluate_movement_or_vibration(value);

    TEST_ASSERT_EQUAL_MESSAGE(true,response,"Not Vibration detected");
}

void test_evaluate_movement_or_vibration_shouldDetect_Movement(void){

    mpuStructData *mpuMeasurements;
    bool response;

    float value = 0.4;

    response = evaluate_movement_or_vibration(value);

    TEST_ASSERT_EQUAL_MESSAGE(false,response,"Not Movement detected");

}


int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_calculate_array_in_movement_shouldSaveValues);
    RUN_TEST(test_f_der_shouldCalculate_VariationCorrectly);
    RUN_TEST(test_evaluate_movement_or_vibration_shouldDetect_Vibration);
    RUN_TEST(test_evaluate_movement_or_vibration_shouldDetect_Movement);
    RUN_TEST(test_calc_dev_estandar_shouldCalculate_devEstandar_Correctly);
    //RUN_TEST(test_mpuLocationChanged_should_DetectVibration);
    
    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}