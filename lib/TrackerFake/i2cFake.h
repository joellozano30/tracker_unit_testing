#pragma once

#include "TrackerFake.h"

struct i2cFake_ {
    virtual void i2cInit(uint8_t);
    virtual void i2cWrite(uint8_t, uint8_t , uint8_t);
    virtual int16_t i2cReadRegister16(uint8_t);
    virtual void i2cReadValueArray(uint8_t , uint8_t , uint8_t , uint8_t );

};

class i2cClass {
 public:

  virtual void i2cInit(uint8_t slaveAddress);
  virtual void i2cWrite(uint8_t deviceAddress, uint8_t regAddress, uint8_t data);
  virtual int16_t i2cReadRegister16(uint8_t reg);
  virtual void i2cReadValueArray(uint8_t deviceAddress, uint8_t regAddress, uint8_t *array, uint8_t arraySize);

 private:
};

extern i2cClass i2c;

