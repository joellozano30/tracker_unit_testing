#pragma once

#include "TrackerFake.h"

struct gpsFake {
    virtual float lat() = 0;
    virtual float lng() = 0;
    virtual float encode(int);
    virtual int isUpdated();
};


class gpsClass {
 public:
  // Initialize the SPI library
  virtual float lat();
  virtual float lng();

  virtual float encode(int);
  virtual int isUpdated();

//   // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
//   virtual uint8_t transfer(uint8_t data);
//   virtual void transfer(void *buf, size_t count);

//   // After performing a group of transfers and releasing the chip select
//   // signal, this function allows others to access the SPI bus
//   virtual void endTransaction(void);

//   //  virtual ~SPIClass();

 private:
};

extern gpsClass gps;

