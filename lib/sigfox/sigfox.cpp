#include "sigfox.h"
//#include "mpu.h"

char sigfoxRxBuffer[50] = {0};
char sigfoxID[51] = {0};
char sigfoxPAC[51] = {0};


void sigfoxInit(void)
{
    #ifndef TEST
    Serial1.begin(9600, SERIAL_8N1, 4, 2);
    #else
    Serial.begin(9600); //Asumming configs
    #endif
    pinMode(SIGFOX_ENABLE, OUTPUT);
    digitalWrite(SIGFOX_ENABLE, LOW);
    sigfoxChangeToRegion4();
}

void sigfoxChangeToRegion4(void)
{
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);

    #ifndef TEST
    Serial1.print("AT$DR=922300000");
    Serial1.print("\r\n");
    Serial1.print("ATS400=<00000000><F0000000><0000001F>,63");
    Serial1.print("\r\n");
    Serial1.print("AT$WR");
    Serial1.print("\r\n");
    Serial1.print("AT$RC");
    Serial1.print("\r\n");
    #else
    Serial.print("AT$DR=922300000");
    Serial.print("\r\n");
    Serial.print("ATS400=<00000000><F0000000><0000001F>,63");
    Serial.print("\r\n");
    Serial.print("AT$WR");
    Serial.print("\r\n");
    Serial.print("AT$RC");
    Serial.print("\r\n");
    #endif

    delay(1000);
    
    #ifndef TEST
    Serial.println("Cambiado zona 4");
    #endif

    digitalWrite(SIGFOX_ENABLE, LOW);
}

void sigfoxReadInfo(void)
{
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);
    sigfoxSendATCommand("AT");
    sigfoxSendATCommand("AT$I=10");
    strcpy(sigfoxID, sigfoxRxBuffer);

    sigfoxSendATCommand("AT$I=11");
    strcpy(sigfoxPAC, sigfoxRxBuffer);

    sigfoxSendATCommand("AT$RC");
    digitalWrite(SIGFOX_ENABLE, LOW);

    #ifdef SIGFOX_DEBUG
        Serial.print("\n\n\n\rID: ");
        Serial.println(sigfoxID);
        Serial.print("PAC: ");
        Serial.println(sigfoxPAC);
    #endif
}

void sigfoxSendATCommand(char* atCmd)
{
    unsigned long rxIdx=0;
    memset(sigfoxRxBuffer, '\0', sizeof(sigfoxRxBuffer));

    // flush data that might remain on buffer
    #ifndef TEST
    while(Serial1.available() > 0)
        Serial1.read();
    
    Serial1.println(atCmd);
    #else
    while(Serial.available() > 0)
        Serial.read();
    
    Serial.print(atCmd);
    #endif


    #ifdef SIGFOX_DEBUG
        Serial.println(atCmd);
    #endif
    delay(1000);

    while(true)
    {   
        #ifndef TEST
        if(Serial1.available() > 0)
        {
            sigfoxRxBuffer[rxIdx] = Serial1.read();
        #else
        if(Serial.available() > 0)
        {
            sigfoxRxBuffer[rxIdx] = Serial.read();
        #endif
            rxIdx++;
            if(strstr(sigfoxRxBuffer, "\n") != NULL)
            {
                #ifdef SIGFOX_DEBUG
                    Serial.print("Respuesta recibida: ");
                    Serial.println(sigfoxRxBuffer);
                #endif
                break;
            }
            yield();
        }
    }
}

void sigfoxSendAlert(alert_type sigfox_alert)
{
    String alert_msg = "AT$SF=";

    switch(sigfox_alert)
    {
        case POWER_OFF:
            alert_msg += "01";
            sigfoxSendMsg(alert_msg);
            break;
        case MOVEMENT_DETECTED:
            alert_msg += "04";
            sigfoxSendMsg(alert_msg);
            break;
        default:
            break;
    }
}

void sigfoxPackGeolocationMessage(uint8_t *mac1, uint8_t *mac2, String *geolocationMsg)
{
    String newMacByte = "";
    int idx = 0;

    *geolocationMsg = "AT$SF=";

    for(idx=0; idx < 6; idx++)
    {
        newMacByte = String(mac1[idx], HEX);
        if(mac1[idx] < 16)
            newMacByte = "0" + newMacByte;
        
        *geolocationMsg += newMacByte;
    }

    for(idx = 0; idx < 6; idx++)
    {
        newMacByte = String(mac2[idx], HEX);
        if(mac2[idx] < 16)
            newMacByte = "0" + newMacByte;
        
        *geolocationMsg += newMacByte;
    }
}

void sigfoxPackGPSMsg(float lat, float lng, float temperature, String *geolocationMsg)
{
    uint8_t *floatPtr;
    String newLatByte, newLngByte, newTempByte;

    *geolocationMsg = "AT$SF=";

    int idx = 0;

    floatPtr = (uint8_t *)&lat;
    #ifdef GPS_PACK_MSG_DEBUG
    Serial.println("[*] Generating Latitude values");
    #endif
    for(idx = 0; idx < 4; idx++)
    {
        newLatByte = String(*(floatPtr + idx), HEX);
        if(*(floatPtr + idx) <16)
            newLatByte = "0" + newLatByte;
        #ifdef GPS_PACK_MSG_DEBUG
        Serial.print("[*] Byte number "); Serial.print(idx); Serial.print(" value: "); Serial.println(newLatByte);
        #endif
        *geolocationMsg += newLatByte;
    }

    floatPtr = (uint8_t *)&lng;
    #ifdef GPS_PACK_MSG_DEBUG
    Serial.println("[*] Generating Longitude values");
    #endif
    for(idx = 0; idx < 4; idx++)
    {
        newLngByte = String(*(floatPtr + idx), HEX);
        if(*(floatPtr + idx) < 16)
            newLngByte = "0" + newLngByte;
        #ifdef GPS_PACK_MSG_DEBUG
        Serial.print("[*] Byte number "); Serial.print(idx); Serial.print(" value: "); Serial.println(newLatByte);
        #endif
        *geolocationMsg += newLngByte;
    }

    floatPtr = (uint8_t *)&temperature;
    #ifdef GPS_PACK_MSG_DEBUG
    Serial.println("[*] Generating Longitude values");
    #endif
    for(idx = 0; idx < 4; idx++)
    {
        newTempByte = String(*(floatPtr + idx), HEX);
        if(*(floatPtr + idx) < 16)
            newTempByte = "0" + newTempByte;
        #ifdef GPS_PACK_MSG_DEBUG
        Serial.print("[*] Byte number "); Serial.print(idx); Serial.print(" value: "); Serial.println(newLatByte);
        #endif
        *geolocationMsg += newTempByte;
    }

    Serial.print("[*] Sigfox message: "); Serial.println(*geolocationMsg);
}

void sigfoxSendMsg(String buf_tx)
{
    //Adding line break
    buf_tx += "\n";

    //******************************
    //Enable Sigfox Module
    digitalWrite(SIGFOX_ENABLE, HIGH);
    delay(1000);

    #ifndef TEST
    //Channel reset to ensure correct frequency
    Serial1.print("AT$RC\n");
    
    //******************************
    //Sending data on Sigfox
    Serial1.print(buf_tx);
    #else
    Serial.write("AT$RC\n");
    Serial.print(buf_tx);
    #endif


    #ifdef SIGFOX_DEBUG
        Serial.print(buf_tx);
    #endif
    delay(3000);

    //disable sigfox module
    digitalWrite(SIGFOX_ENABLE, LOW);
}

// void sigfoxSendMsg(String sigfoxTxBuffer)
// {
//     // Adding newline to message
//     sigfoxTxBuffer += "\n";

//     //**************************
//     // Enabling Sigfox module
//     digitalWrite(SIGFOX_ENABLE, HIGH);
//     delay(1000);

//     // Reset channel to ensure that message is sent in the right frequency
//     #ifdef SIGFOX_DEBUG
//         Serial.print("AT$RC\n");
//     #endif
//     Serial1.print("AT$RC\n");

//     //**************************
//     // Sending the message via Sigfox
//     #ifdef SIGFOX_DEBUG
//         Serial.print(sigfoxTxBuffer);
//     #endif
//     Serial1.print(sigfoxTxBuffer);
//     delay(3000);
//     // Disabling Sigfox module
//     digitalWrite(SIGFOX_ENABLE, LOW);

//     // Flushing the buffer
//     while(Serial1.available() > 0)
//         Serial1.read();
// }