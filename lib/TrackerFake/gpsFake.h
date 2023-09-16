#pragma once

#include "TrackerFake.h"

struct gpsFake_ {
    virtual float lat() = 0;
    virtual float lng() = 0;
    virtual float encode(int);
    virtual int isUpdated();
};

class gpsClass {
 public:
  
  virtual float lat();
  virtual float lng();

  virtual float encode(int value);
  virtual int isUpdated();

 private:
};


extern gpsClass gps;

