#include <Arduino.h>
#include <unity.h>
#include "mpu.h"

void setUp(void) {
    // set stuff up here
    mpuInit();
}

void tearDown(void) {
    // clean stuff up here
}

void test_mpu(void) {

    mpuStructData mpuMeasurements = mpuGetLocationData();

    TEST_ASSERT_EQUAL_FLOAT(0.9,mpuMeasurements.modulo);
}

void setup()
{
    //delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_mpu);

    UNITY_END(); // stop unit testing
}

void loop()
{

}