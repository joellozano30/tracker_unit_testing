#pragma once

#include <stdio.h>
#include <unordered_map>  
#include <cstring>
#include <cstdint>
#include <stdexcept>
#include "fakeit.hpp"

#include "gpsFake.h"
#include "i2cFake.h"

#define TrackerFake(mock) _TrackerFakeGet##mock()

#define TrackerFakeReset() \
    getTrackerFakeContext()->reset()

#define TrackerFakeInstance(mock, ...) \
    getTrackerFakeContext()->mock(__VA_ARGS__)

#define TrackerFakeMock(mock) \
    new mock##FakeProxy(TrackerFakeInstance(mock))

#define _TrackerFakeGetMock(mock) \
    getTrackerFakeContext()->Mocks->mock

#define _TrackerFakeGetgps() _TrackerFakeGetMock(gps)
#define _TrackerFakeGeti2c() _TrackerFakeGetMock(i2c)

#define _TrackerFakeInstanceGetter1(mock) \
    mock##Fake_* mock() \
    { \
        if (!this->Instances->mock){ \
            this->Instances->mock = &this->Mocks->mock.get(); \
        } \
        return this->Instances->mock; \
    }

struct TrackerFakeMocks
{
    fakeit::Mock<gpsFake_> gps;
    fakeit::Mock<i2cFake_> i2c;
};

struct TrackerFakeInstances
{
    gpsFake_* gps;
    i2cFake_* i2c;
};

class TrackerFakeContext
{
    public:
        TrackerFakeInstances* Instances = new TrackerFakeInstances();
        TrackerFakeMocks* Mocks = new TrackerFakeMocks();
        std::unordered_map<void*, void*> Mapping;

        _TrackerFakeInstanceGetter1(gps)
        _TrackerFakeInstanceGetter1(i2c)

        TrackerFakeContext()
        {
            this->reset();
        }

        void reset(void)
        {
            this->Instances = new TrackerFakeInstances();

            this->Mocks->gps.Reset();
            this->Mocks->i2c.Reset();
        }
};

TrackerFakeContext* getTrackerFakeContext();
