#include <unity.h>        // Incluye la biblioteca de Unity
#include "sigfox.h"
#include "mpu.h"

void setUp(void) {
    // set stuff up here
    sigfoxInit();
}

void tearDown(void) {
    // clean stuff up here
}

void test_sigfoxSendMsg_should_beAbleTo_SendMessagesCorrectly(){

    const char* packedMessage = "AT$SF=d63c1042"; //Data
    
    sigfoxSendMsg(packedMessage);
}

void test_sigfoxPackGPSMsg_should_PackMessagesCorrectly(){
    float lat, lng, temperature;
    uint8_t *floatPtr;
    String newLatByte;

    lat = -77.233; //4c779ac2
    lng = -12.545; //52b848c1
    temperature = 25.64; //b81ecd41

    String MessageTopack;
    
    sigfoxPackGPSMsg(lat,lng,temperature,&MessageTopack);

    const char *MessageTopack_c = MessageTopack.c_str();

    TEST_ASSERT_EQUAL_STRING("AT$SF=4c779ac252b848c1b81ecd41",MessageTopack_c);
}


void setup() {
    UNITY_BEGIN(); // Inicializa Unity Test Framework

    // Ejecuta las pruebas definidas
    RUN_TEST(test_sigfoxSendMsg_should_beAbleTo_SendMessagesCorrectly);
    RUN_TEST(test_sigfoxPackGPSMsg_should_PackMessagesCorrectly);

    UNITY_END(); // Finaliza Unity Test Framework
}

void loop(){
    // Executes Nothing
}