#include "TrackerFake.h"

TrackerFakeContext* trackerFakeContext;

TrackerFakeContext* getTrackerFakeContext()
{
    if (!trackerFakeContext) {
        trackerFakeContext = new TrackerFakeContext();
    }
    return trackerFakeContext;
}
