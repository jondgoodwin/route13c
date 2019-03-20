///////////////////////////////////////////////////////////////////////////////
//
// ILocation
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

typedef int32_t LocationId;

typedef uint32_t SimTime;

class ILocations {
public:
    ILocations(int32_t count) : m_count(count) {};

    int32_t getCount() { return m_count; }

    // Calculate the transit time between two locations
    virtual SimTime transitTimeEstimator(LocationId origin, LocationId destination) = 0;

    // Return the next location along the path from the specified origin to the destination.
    virtual LocationId routeNextStep(LocationId origin, LocationId destination) = 0;

private:
    int32_t m_count;
};
