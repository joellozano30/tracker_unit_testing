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
    i2cInit(MPU6050_SLAVE_ADDRESS);
    delay(150);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_CONFIG, 0x00);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_GYRO_CONFIG, 0x00);  // set +/-250 degree/second full scale
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_CONFIG, 0x00); // set +/- 2g full scale
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_FIFO_EN, 0x00);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_INT_ENABLE, 0x01);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
    i2cWrite(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_USER_CTRL, 0x00);

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
    i2cReadValueArray(deviceAddress, regAddress, array, arraySize);

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
    // Variables agregadas
    static uint32_t init_time_evaluate;
    // static uint32_t init_time_evaluate_movement;
    static uint32_t calibration_time = millis();

    float acc_module;
    float var_vel = 0;
    float pos_init = 0;
    float var_acel = 0;
    float modulo_init = 0;
    // float suma=0;
    // float acum=0;
    int i = 0;
    uint8_t flag_evaluation = 0;
    mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
    mpuConvertRawValues(mpuMeasurements);

    if(((millis() - calibration_time) >= MPU6050_TIME_TO_CALIBRATE) && flag_set_ubication){
        Axi = mpuMeasurements->Ax;
        Ayi = mpuMeasurements->Ay;
        Azi = mpuMeasurements->Az;
        Serial.println(" ");
        Serial.println("Valores nuevos normalizados: ");
        Serial.print("Axi: ");
        Serial.println(Axi);
        Serial.print("Ayi: ");
        Serial.println(Ayi);
        Serial.print("Azi: ");
        Serial.println(Azi);
        Serial.println(" ");
        calibration_time = millis();
    }

    if(Axi == 0 || Ayi == 0 || Azi == 0){
        Serial.println("-------------------------------");
        Serial.println("Valores normalizados iniciales: ");
        Serial.println("-------------------------------");
        mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
        mpuConvertRawValues(mpuMeasurements);
        Axi = mpuMeasurements->Ax;
        Ayi = mpuMeasurements->Ay;
        Azi = mpuMeasurements->Az;
        Serial.print("Axi: ");
        Serial.println(Axi);
        Serial.print("Ayi: ");
        Serial.println(Ayi);
        Serial.print("Azi: ");
        Serial.println(Azi);
        Serial.println(" ");
    }

    mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));

    // Impresion
    if ((millis() - lastTimeRead) >= 500)
    {
        #ifdef MPU_DEBUG
        mpuPrintReadings(mpuMeasurements);
        #endif
        lastTimeRead = millis();
    }
    // Serial.println(flag_evaluation);


    if (mpuMeasurements->modulo > MPU6050_MODULO_VECTOR_LOWER_LIMIT)
    {   
        if(flag_evaluate_movement){
            // Se guarda último módulo usado para entrar a la evaluación
            mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
            mpuConvertRawValues(mpuMeasurements);
            //mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));
            mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax), 2) + pow((mpuMeasurements->Ay), 2) + pow((mpuMeasurements->Az), 2));
            modulo_init = mpuMeasurements->modulo;
            //GUARDAMOS MODULO INICIAL

            flag_evaluate_movement = 0;
        }
        

        flag_set_ubication = 0;
        
        Serial.println("MPU is detecting an unexpected variation in its measurements!.");
        Serial.println("Evaluation started.");
        // Serial.println("Esta evaluando si excede el modulo");

        // Reinicio del vector aceleración
        for (i = 0; i <= (MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW); i++)
        {
            acc_module_vector[i] = 0;
        }
        // Reiniciamos i
        i = 0;
        init_time_evaluate = millis();

        while ((millis() - init_time_evaluate) < MPU6050_TIME_BELOW_VECTOR_LIMIT)
        {
            if ((millis() - lastTimeRead) >= MPU6050_SAMPLE_TIME_IN_WINDOW)
            {
                // Serial.println("Hola");
                mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
                mpuConvertRawValues(mpuMeasurements);

                mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));
            
                lastTimeRead = millis();

                acc_module = mpuMeasurements->modulo;

                /*if (mpuMeasurements->modulo > MPU6050_MODULO_VECTOR_LOWER_LIMIT)
                {
                    counter_false_movements++;
                }
                */
                acc_module_vector[i] = acc_module;
                i++;
            }
        }

        
        i++;
        mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
        mpuConvertRawValues(mpuMeasurements);

        mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));

        acc_module = mpuMeasurements->modulo;
        acc_module_vector[i] = acc_module;
        

        Serial.println("Evaluation Finished.");

        mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
        mpuConvertRawValues(mpuMeasurements);
        //mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax - Axi), 2) + pow((mpuMeasurements->Ay - Ayi), 2) + pow((mpuMeasurements->Az - Azi), 2));
        mpuMeasurements->modulo = sqrt(pow((mpuMeasurements->Ax), 2) + pow((mpuMeasurements->Ay), 2) + pow((mpuMeasurements->Az), 2));

        //mpuPrintReadings(mpuMeasurements);

        
        if(fabs((mpuMeasurements->modulo) - MPU6050_MODULO_VECTOR_LOWER_LIMIT_STATIC) <= 0.1){
            Serial.println("-------------------------------------------------------");
            /*
            Serial.println("------------------------");
            Serial.println("----- False alarm. -----");
            Serial.println("------------------------");
            */

            mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
            mpuConvertRawValues(mpuMeasurements);
            Axi = mpuMeasurements->Ax;
            Ayi = mpuMeasurements->Ay;
            Azi = mpuMeasurements->Az;
            Serial.println("-------------------------------------------------------");
            Serial.println("Equipo dejo de moverse!!. Valores nuevos normalizados: ");
            Serial.println("-------------------------------------------------------");
            Serial.print("Axi: ");
            Serial.println(Axi);
            Serial.print("Ayi: ");
            Serial.println(Ayi);
            Serial.print("Azi: ");
            Serial.println(Azi);
            Serial.println(" ");

            flag_set_ubication = 1;
            flag_evaluate_movement = 1;
            //return false;
        }
        
        /*
        Serial.print("Vector Acc: ");
        Serial.print(" ");
        i=0;
        for(i=0;i<(MPU6050_TIME_BELOW_VECTOR_LIMIT/MPU6050_SAMPLE_TIME_IN_WINDOW);i++){
            Serial.print(acc_module_vector[i]);
            Serial.print(" ");
        }
        Serial.println(" ");
        */

        // Impresión Vector Velocidad
        /*
        Serial.println(" ");
        //Serial.println(flag_evaluation);
        Serial.print("Vector Vel: ");
        f_intgr(acc_module_vector,(int)(MPU6050_TIME_BELOW_VECTOR_LIMIT/MPU6050_SAMPLE_TIME_IN_WINDOW),MPU6050_SAMPLE_TIME_IN_WINDOW);
        i=0;
        for(i=0;i<(MPU6050_TIME_BELOW_VECTOR_LIMIT/MPU6050_SAMPLE_TIME_IN_WINDOW);i++){
            Serial.print(acc_module_vector[i]);
            Serial.print(" ");
        }
        */

        var_acel = f_der(acc_module_vector, (int)(MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW));
        
        
        Serial.println(" ");
        Serial.print("Variación de aceleracion (cm/s^2): ");
        Serial.print((var_acel));
        Serial.println(" ");
        
        
        f_intgr(acc_module_vector, (int)(MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW), MPU6050_SAMPLE_TIME_IN_WINDOW);
        var_vel = acc_module_vector[(int)(MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW) - 1];

        
        Serial.println(" ");
        Serial.print("Variación de velocidad (cm/s): ");
        Serial.print((var_vel));
        Serial.println(" ");
        

        
        f_intgr(acc_module_vector, (int)(MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW), MPU6050_SAMPLE_TIME_IN_WINDOW);
        pos_init = acc_module_vector[(int)(MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW) - 1];
        Serial.println(" ");
        Serial.print("Variación de posicion (cm): ");
        Serial.print((pos_init));
        Serial.println(" ");
        

        // Impresión Vector Posición
        /*
        Serial.print("Vector Pos: ");
        f_intgr(acc_module_vector,(MPU6050_TIME_BELOW_VECTOR_LIMIT/MPU6050_SAMPLE_TIME_IN_WINDOW),MPU6050_SAMPLE_TIME_IN_WINDOW);
        i=0;
        for(i=0;i<(MPU6050_TIME_BELOW_VECTOR_LIMIT/MPU6050_SAMPLE_TIME_IN_WINDOW);i++){
            Serial.print(acc_module_vector[i]);
            Serial.print(" ");
        }
        Serial.println(" ");
        */ 
       
        /*
        f_intgr(acc_module_vector, (int)(MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW), MPU6050_SAMPLE_TIME_IN_WINDOW);
        pos_init = acc_module_vector[(int)(MPU6050_TIME_BELOW_VECTOR_LIMIT / MPU6050_SAMPLE_TIME_IN_WINDOW) - 1];
        Serial.println(" ");
        Serial.print("Variación de posicion (cm): ");
        Serial.print((pos_init));
        Serial.println(" ");
        */

        if((var_acel > MPU6050_ACELERATION_VARIATION_LIMIT)) //Detecta sobre aceleracion
            flag_evaluation = 1;

    }
    else{
        flag_set_ubication = 1;
        return false;
    }
        

    /*Serial.print("Numero de movimientos falsos: ");
    Serial.print(" ");
    Serial.print(counter_false_movements);
    Serial.println(" ");
    */

    /*
    if(counter_false_movements>MPU6050_COUNTER_FALSE_ALARM)
        flag_evaluation = 1;
    else
        flag_evaluation = 0;
    */
    Serial.println(" ");

    if(flag_evaluation)
    {   
        Serial.println("------------------------");
        Serial.println("----- False alarm. -----");
        Serial.println("------------------------");
        return false;
    }
    else
    {
        Serial.println(" ");
        Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
        Serial.println("%%%%%% Movement detected. %%%%%%");
        Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
        Serial.println("MPU values that have generated changes in the FSM: Idle -> Location: ");
        return false;
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

float mpuGetTemperature(void)
{
    mpuStructData mpuMeasurements;
    float temperature;

    mpuReadRawValue(MPU6050_SLAVE_ADDRESS, MPU6050_REGISTER_ACCEL_XOUT_H);
    mpuConvertRawValues(&mpuMeasurements);
    temperature = mpuMeasurements.T;

    return temperature;
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
    return i2cReadRegister16(MPU6050_REG_ACCEL_XOFFS_H);
}

int16_t mpuGetAccelOffsetY(void)
{
    return i2cReadRegister16(MPU6050_REG_ACCEL_YOFFS_H);
}

int16_t mpuGetAccelOffsetZ(void)
{
    return i2cReadRegister16(MPU6050_REG_ACCEL_ZOFFS_H);
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