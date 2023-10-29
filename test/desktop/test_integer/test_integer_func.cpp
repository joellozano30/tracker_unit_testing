#include <unity.h>        // Incluye la biblioteca de Unity
#include "integer.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_f_der_shouldCalculate_VariationCorrectly(void){
    float acc_module_vector[10] = {0.2,0.4,0.0,0.9,1.0,1.1,1.3,1.4,0.5,0.6};
    float var_acel;
    int eval_var_acel=0;

    var_acel = f_der(acc_module_vector, 10);
    eval_var_acel = (var_acel -1.4)<0.000001;

    TEST_ASSERT_EQUAL_MESSAGE(1,eval_var_acel,"AAAA");
}

int main() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_f_der_shouldCalculate_VariationCorrectly);
    
    UNITY_END(); // Finaliza Unity Test Framework

    return 0;
}