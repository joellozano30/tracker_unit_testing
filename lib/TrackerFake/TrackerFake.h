#pragma once

#include <unordered_map>  
#include <cstring>
#include <cstdint>
#include <stdexcept>
#include "fakeit.hpp"

#include "gpsFake.h"

#define TrackerFake(mock) _TrackerFakeGet##mock()

#define TrackerFakeReset() \
    getTrackerFakeContext()->reset()

#define ArduinoFakeInstance(mock, ...) \
    getTrackerFakeContext()->mock(__VA_ARGS__)

#define TrackerFakeMock(mock) \
    new mock##FakeProxy(TrackerFakeInstance(mock))

#define _TrackerFakeGetMock(mock) \
    getTrackerFakeContext()->Mocks->mock

#define _TrackerFakeGetFunction() _TrackerFakeGetMock(gps)

#define _TrackerFakeInstanceGetter1(mock) \
    mock##Fake* mock() \
    { \
        if (!this->Instances->mock){ \
            this->Instances->mock = &this->Mocks->mock.get(); \
        } \
        return this->Instances->mock; \
    }

// #define _ArduinoFakeInstanceGetter2(name, clazz) \
//     name##Fake* name(class clazz* instance) \
//     { \
//         if (Mapping[instance]) { \
//             return (name##Fake*) Mapping[instance]; \
//         } \
//         if (dynamic_cast<name##FakeProxy*>(instance)) { \
//             return dynamic_cast<name##FakeProxy*>(instance)->get##name##Fake(); \
//         } \
//         throw std::runtime_error("Unknown instance"); \
//     }

struct TrackerFakeMocks
{
    fakeit::Mock<gpsFake> gps;
};

struct TrackerFakeInstances
{
    gpsFake* gps;
};

class TrackerFakeContext
{
    public:
        TrackerFakeInstances* Instances = new TrackerFakeInstances();
        TrackerFakeMocks* Mocks = new TrackerFakeMocks();
        std::unordered_map<void*, void*> Mapping;

        _TrackerFakeInstanceGetter1(gps)

        TrackerFakeContext()
        {
            this->reset();
        }

        void reset(void)
        {
            this->Instances = new TrackerFakeInstances();

            this->Mocks->gps.Reset();
        }
};

TrackerFakeContext* getTrackerFakeContext();

// clang-format on