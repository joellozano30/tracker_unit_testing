#include <unity.h>        // Incluye la biblioteca de Unity
#include "mpu.h"
#include "integer.h"

void setUp(void) {
    // set stuff up here
    mpuInit();
}

void tearDown(void) {
    // clean stuff up here
}

void test_calculate_array_in_movement_shouldSaveValues(void){
    mpuStructData mpuMeasurements;
    int num_elements=0;
    int evaluate = false;


    num_elements = get_num_elements_of_acc_module();
    TEST_ASSERT_EQUAL_MESSAGE(0,num_elements,"Elements existing in array");

    calculate_acc_module_array_in_movement(&mpuMeasurements);

    num_elements = get_num_elements_of_acc_module();
    evaluate = num_elements > 0;
    TEST_ASSERT_EQUAL_MESSAGE(true,evaluate,"Elements where not saved in array");
}

void test_f_der_shouldCalculate_VariationCorrectly(void){
    float acc_module_vector[10] = {0.2,0.4,0.0,0.9,1.0,1.1,1.3,1.4,0.5,0.6};
    float var_acel;
    int eval_var_acel=false;

    var_acel = f_der(acc_module_vector, 10);
    eval_var_acel = (var_acel -1.4)<0.000001;

    TEST_ASSERT_EQUAL_MESSAGE(true,eval_var_acel,"Error in calculus");
}


void test_evaluate_movement_or_vibration_shouldDetect_Vibration(void){

    mpuStructData *mpuMeasurements;
    bool response;

    float value = 0.9;

    response = evaluate_movement_or_vibration(value);//mpuLocationChanged(mpuMeasurements);//evaluate_movement_or_vibration(value);

    TEST_ASSERT_EQUAL_MESSAGE(true,response,"Not Vibration detected");
}

void test_evaluate_movement_or_vibration_shouldDetect_Movement(void){

    mpuStructData *mpuMeasurements;
    bool response;

    float value = 0.4;

    response = evaluate_movement_or_vibration(value);

    TEST_ASSERT_EQUAL_MESSAGE(false,response,"Not Movement detected");

}

void setup() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_calculate_array_in_movement_shouldSaveValues);
    RUN_TEST(test_f_der_shouldCalculate_VariationCorrectly);
    RUN_TEST(test_evaluate_movement_or_vibration_shouldDetect_Vibration);
    RUN_TEST(test_evaluate_movement_or_vibration_shouldDetect_Movement);
    
    UNITY_END(); // Finaliza Unity Test Framework
}

void loop(){
    // Executes Nothing
}