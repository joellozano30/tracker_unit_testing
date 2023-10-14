#include <Arduino.h>
#include <unity.h>
#include "mpu.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_function(void) {
    // include unit to test 
}

int main(){

    UNITY_BEGIN(); // start unit testing

    RUN_TEST(test_function);

    UNITY_END(); // stop unit testing

}

void setup()
{
    //delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_function);

    UNITY_END(); // stop unit testing
}

void loop()
{

}