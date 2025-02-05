#ifndef MPU_H
#define MPU_H

#ifndef TEST
    #include "Arduino.h"
    #include "MPU6050.h"
    #include "i2c.h"
#else
    #include "ArduinoFake.h"
    #include <math.h>
    #include <stdio.h>
    #include <string.h>
    
    using namespace fakeit;
    #include "TrackerFake.h"
#endif


#include "config.h"
#include "gps.h"

//Direcciones de registros de configuracion del dispositivo
#define MPU6050_REG_ACCEL_XOFFS_H     (0x06)
#define MPU6050_REG_ACCEL_XOFFS_L     (0x07)
#define MPU6050_REG_ACCEL_YOFFS_H     (0x08)
#define MPU6050_REG_ACCEL_YOFFS_L     (0x09)
#define MPU6050_REG_ACCEL_ZOFFS_H     (0x0A)
#define MPU6050_REG_ACCEL_ZOFFS_L     (0x0B)

//Direcciones de registros de configuracion del dispositivo
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

#define MPU6050_SLAVE_ADDRESS   0x68//0x69
//const uint8_t MPU6050_SLAVE_ADDRESS  = 0x68;//0x69
#define MPU6050_AX_LOWER_LIMIT  4
#define MPU6050_AY_LOWER_LIMIT  4
#define MPU6050_AZ_LOWER_LIMIT  4

#define MPU6050_ACELERATION_VARIATION_LIMIT 0.1 //Unidades en cm/S^2
#define MPU6050_VELOCITY_VARIATION_LIMIT 0.8 //Unidades en cm/s
#define MPU6050_POSITION_VARIATION_LIMIT 4 //Unidades en cm

#define MPU6050_MODULO_VECTOR_LOWER_LIMIT_STATIC 1
#define MPU6050_MODULO_VECTOR_LOWER_LIMIT 0.2
#define MPU6050_TIME_TO_CALIBRATE 4000

#define MPU6050_EVALUATION_TIME 2000 //Tiempo utilizado. Unidades en ms.
#define MPU6050_TIME_BELOW_VECTOR_LIMIT 300 //Tiempo utilizado para la ventana de tiempo. Unidades en ms.
#define MPU6050_SAMPLE_TIME_IN_WINDOW 2 //Tiempo entre evaluaciones en ventana de tiempo. Unidades en ms.
#define MPU6050_COUNTER_FALSE_ALARM 20


typedef struct mpuData {
    double Ax;
    double Ay;
    double Az;
    double T;
    double Gx;
    double Gy;
    double Gz;
    double modulo;
}mpuStructData;

void mpuInit(void);
void mpuReadRawValue(uint8_t deviceAddress, uint8_t regAddress);
bool mpuLocationChanged(mpuStructData *mpuMeasurements);
int get_num_elements_of_acc_module(void);
void calculate_acc_module_array_in_movement(mpuStructData *mpuMeasurements);
void set_acc_reference_values(mpuStructData *mpuMeasurements);
void set_recalibrate(int value);
bool evaluate_movement_or_vibration(float var_acel);
mpuStructData mpuGetLocationData(void);
float getTemperatureData(void);
void mpuConvertRawValues(mpuStructData *mpuMeasurments);
int16_t mpuGetAccelOffsetX(void);
int16_t mpuGetAccelOffsetY(void);
int16_t mpuGetAccelOffsetZ(void);
void mpuPrintReadings(mpuStructData *mpuMeasurements);

#endif