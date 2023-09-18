#pragma once

#include "TrackerFake.h"

struct gpsFake_ {
    virtual double lat() = 0;
    virtual double lng() = 0;
    virtual float encode(int);
    virtual int isUpdated();
};

class gpsClass {
 public:
  
  virtual double lat();
  virtual double lng();

  virtual float encode(int value);
  virtual int isUpdated();

 private:
};


extern gpsClass gps;

