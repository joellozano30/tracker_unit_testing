#include "mpu.h"
#include "integer.h"

static int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

// sensitivity scale factor respective to full scale setting provided in datasheet
// Valores de factor de sensibilidad, dependen de la hoja tecnica del dispositivo
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;
float limit_value = 0;
int counter_false_movements = 0;
int flag_set_ubication = 1;
int flag_evaluate_movement = 1;

//Aceleraciones iniciales
float Axi=0;
float Ayi=0;
float Azi=0;

float *acc_module_vector = (float *)malloc((MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW) * sizeof(float));

void mpuInit(void)
{
    #ifndef TEST
    i2cInit(MPU6050_SLAVE_ADDRESS);
    delay(150);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_CONFIG, 0x00);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_GYRO_CONFIG, 0x00);  // set +/-250 degree/second full scale
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_CONFIG, 0x00); // set +/- 2g full scale
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_FIFO_EN, 0x00);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_INT_ENABLE, 0x01);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_USER_CTRL, 0x00);
    #else
    i2c.i2cInit(MPU6050_SLAVE_ADDRESS);
    delay(150);
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_CONFIG, 0x00);
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_GYRO_CONFIG, 0x00);  // set +/-250 degree/second full scale
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_CONFIG, 0x00); // set +/- 2g full scale
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_FIFO_EN, 0x00);
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_INT_ENABLE, 0x01);
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
    i2c.i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_USER_CTRL, 0x00);
    #endif

#ifdef PRINT_ACCELEROMETER_INIT
    Serial.print(" * Accelerometer offsets: ");
    Serial.print(mpu_getAccelOffsetX());
    Serial.print(" / ");
    Serial.print(mpu_getAccelOffsetY());
    Serial.print(" / ");
    Serial.println(mpu_getAccelOffsetZ());

    Serial.println();
#endif
}

void mpuReadRawValue(uint8_t deviceAddress, uint8_t regAddress)
{
    uint8_t arraySize = 14;
    uint8_t array[arraySize];

    #ifndef TEST
    i2cReadValueArray(deviceAddress, regAddress, array, arraySize);
    #else
    i2c.i2cReadValueArray(deviceAddress, regAddress, array, arraySize);
    #endif

    AccelX = (((int16_t)array[0] << 8) | array[1]);
    AccelY = (((int16_t)array[2] << 8) | array[3]);
    AccelZ = (((int16_t)array[4] << 8) | array[5]);
    Temperature = (((int16_t)array[6] << 8) | array[7]);
    GyroX = (((int16_t)array[8] << 8) | array[9]);
    GyroY = (((int16_t)array[10] << 8) | array[11]);
    GyroZ = (((int16_t)array[12] << 8) | array[13]);
}

bool mpuLocationChanged(mpuStructData *mpuMeasurements)
{
    static uint32_t lastTimeRead = millis();
    static uint32_t init_time_evaluate;
    static uint32_t calibration_time = millis();

    float acc_module;
    float var_vel = 0;
    float pos_init = 0;
    float var_acel = 0;
    float modulo_init = 0;
    int i = 0;
    uint8_t flag_evaluation = 0;

    mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
    mpuConvertRawValues(mpuMeasurements);

    if(((millis() - calibration_time) >= MPU6050_TIME_TO_CALIBRATE) && flag_set_ubication){
        //Valores nuevos normalizados
        Axi = mpuMeasurements->Ax;
        Ayi = mpuMeasurements->Ay;
        Azi = mpuMeasurements->Az;
        calibration_time = millis();
    }

    if(Axi == 0 || Ayi == 0 || Azi == 0){
        //Valores normalizados iniciales
        mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
        mpuConvertRawValues(mpuMeasurements);
        Axi = mpuMeasurements->Ax;
        Ayi = mpuMeasurements->Ay;
        Azi = mpuMeasurements->Az;
    }

    mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));

    if ((millis() - lastTimeRead) >= 500)
    {
        #ifdef MPU_DEBUG
        mpuPrintReadings(mpuMeasurements);
        #endif
        lastTimeRead = millis();
    }

    // Se detecta movimiento
    if (mpuMeasurements->modulo > MPU6050_MODULO_VECTOR_LOWER_LIMIT)
    {   
        if(flag_evaluate_movement){
            // Se guarda último módulo usado para entrar a la evaluación
            mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
            mpuConvertRawValues(mpuMeasurements);
            mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax), 2) + pow((mpuMeasurements->Ay), 2) + pow((mpuMeasurements->Az), 2));
            //Se lee módulo inicial
            modulo_init = mpuMeasurements->modulo;
            flag_evaluate_movement = 0;
        }

        flag_set_ubication = 0;
        
        Serial.println("MPU is detecting an unexpected variation in its measurements!.");
        Serial.println("Evaluation started.");

        // Reinicio del vector aceleración
        for (i = 0; i <= (MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW); i++)
        {
            acc_module_vector[i] = 0;
        }
        // Reiniciamos i
        i = 0;
        init_time_evaluate = millis();

        //Obtención de arreglo de datos leídos por el módulo acelerómetro                            
        while ((millis() - init_time_evaluate) < MPU6050_TIME_BELOW_VECTOR_LIMIT)
        {
            if ((millis() - lastTimeRead) >= MPU6050_SAMPLE_TIME_IN_WINDOW)
            {
                mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
                mpuConvertRawValues(mpuMeasurements);
                mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));
                lastTimeRead = millis();
                //Obtención vector aceleración
                acc_module = mpuMeasurements->modulo;
                acc_module_vector[i] = acc_module;
                i++;
            }
        }

        i++;
        mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
        mpuConvertRawValues(mpuMeasurements);

        //Se evalúa variación de movimiento a partir de la referencia
        mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));

        acc_module = mpuMeasurements->modulo;
        acc_module_vector[i] = acc_module;

        if(fabs((mpuMeasurements->modulo) - MPU6050_MODULO_VECTOR_LOWER_LIMIT_STATIC) <= 0.1){
  
            mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
            mpuConvertRawValues(mpuMeasurements);
            Axi = mpuMeasurements->Ax;
            Ayi = mpuMeasurements->Ay;
            Azi = mpuMeasurements->Az;

            //Equipo dejo de moverse
            flag_set_ubication = 1;
            flag_evaluate_movement = 1;

            return false;
        }

        // Serial.println("Acceleration Module Vector: ");
        // for (int i = 0; i <= (MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW); ++i) {
        //     Serial.print(acc_module_vector[i]);
        //     Serial.print(" ");
        // }
        // Serial.println(" ");
        
        /* Calculo de variaciones de aceleracion y velocidad */
        var_acel = f_der(acc_module_vector, (int)(MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW));

        if((var_acel > MPU6050_ACELERATION_VARIATION_LIMIT)) //Detecta sobre aceleracion
            flag_evaluation = 1;
    }
    else{
        flag_set_ubication = 1;
        return false;
    }
        
    if(flag_evaluation)
    {   
        /* False Alarm */
        Serial.println("False Alarm");
        return false;
    }
    else
    {
        /* Movement detected */
        Serial.println("MPU values that have generated changes in the FSM: Idle -> Location: ");
        return true;
    }
}

mpuStructData mpuGetLocationData(void)
{
    mpuStructData mpuMeasurements;

    mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
    mpuConvertRawValues(&mpuMeasurements);
    mpuMeasurements.modulo = sqrt(pow(mpuMeasurements.Ax, 2) + pow(mpuMeasurements.Ay, 2) + pow(mpuMeasurements.Az, 2));

    return mpuMeasurements;
}

float getTemperatureData(void){

    float dataTemp = 0;
    mpuStructData mpuMeasurements;
    mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
    mpuConvertRawValues(&mpuMeasurements);

    dataTemp = mpuMeasurements.T;

    return dataTemp;
}

void mpuConvertRawValues(mpuStructData *mpuMeasurments)
{
    mpuMeasurments->Ax = (double)AccelX / AccelScaleFactor;
    mpuMeasurments->Ay = (double)AccelY / AccelScaleFactor;
    mpuMeasurments->Az = (double)AccelZ / AccelScaleFactor;
    mpuMeasurments->T = (double)Temperature / 340 + 36.53; // formula de temperatura
    mpuMeasurments->Gx = (double)GyroX / GyroScaleFactor;
    mpuMeasurments->Gy = (double)GyroY / GyroScaleFactor;
    mpuMeasurments->Gz = (double)GyroZ / GyroScaleFactor;
}

int16_t mpuGetAccelOffsetX(void)
{
    #ifndef TEST
    return i2cReadRegister16(MPU6050_REG_ACCEL_XOFFS_H);
    #else
    return i2c.i2cReadRegister16(MPU6050_REG_ACCEL_XOFFS_H);
    #endif
}

int16_t mpuGetAccelOffsetY(void)
{
    #ifndef TEST
    return i2cReadRegister16(MPU6050_REG_ACCEL_XOFFS_H);
    #else
    return i2c.i2cReadRegister16(MPU6050_REG_ACCEL_YOFFS_H);
    #endif
}

int16_t mpuGetAccelOffsetZ(void)
{
    #ifndef TEST
    return i2cReadRegister16(MPU6050_REG_ACCEL_XOFFS_H);
    #else
    return i2c.i2cReadRegister16(MPU6050_REG_ACCEL_ZOFFS_H);
    #endif
}

void mpuPrintReadings(mpuStructData *mpuMeasurements)
{
    Serial.print(" Ax: ");
    Serial.print(mpuMeasurements->Ax);
    Serial.print(" Ay: ");
    Serial.print(mpuMeasurements->Ay);
    Serial.print(" Az: ");
    Serial.print(mpuMeasurements->Az);
    Serial.print(" Modulo vector: ");
    Serial.print(mpuMeasurements->modulo);
    Serial.print(" T: ");
    Serial.println(mpuMeasurements->T);
}