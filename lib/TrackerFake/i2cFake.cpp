#include "TrackerFake.h"
#include "i2cFake.h"

// void i2cClass::i2cInit(uint8_t slaveAddress) { TrackerFakeInstance(i2c)->i2cInit(slaveAddress); };

// void i2cClass::i2cWrite(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) { TrackerFakeInstance(i2c)->i2cWrite(deviceAddress, regAddress, data); };

// uint16_t i2cClass::i2cReadRegister16(uint8_t reg) { return TrackerFakeInstance(i2c)->i2cReadRegister16(reg); };

// void i2cClass::i2cReadValueArray(uint8_t deviceAddress, uint8_t regAddress, uint8_t *array, uint8_t arraySize) { TrackerFakeInstance(i2c)->i2cReadValueArray(deviceAddress, regAddress, array, arraySize); };

void i2cClass::i2cInit(uint8_t slaveAddress) { TrackerFakeInstance(i2c)->i2cInit(slaveAddress); };

void i2cClass::i2cWrite(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) { TrackerFakeInstance(i2c)->i2cWrite(deviceAddress, regAddress, data); };

int16_t i2cClass::i2cReadRegister16(uint8_t reg) {  TrackerFakeInstance(i2c)->i2cReadRegister16(reg); };

void i2cClass::i2cReadValueArray(uint8_t deviceAddress, uint8_t regAddress, uint8_t *array, uint8_t arraySize) { TrackerFakeInstance(i2c)->i2cReadValueArray(deviceAddress, regAddress, *array, arraySize); };

i2cClass i2c;

