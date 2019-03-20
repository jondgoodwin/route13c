///////////////////////////////////////////////////////////////////////////////
//
// LinearLocations
//
///////////////////////////////////////////////////////////////////////////////

#include "LinearLocations.h"

LinearLocations::LinearLocations(int32_t locs, SimTime distance) :
    ILocations(locs),
    m_distance(distance)
{
};

// The transitTimeEstimator models a sequence of locations along a line.
// The travel time between two locations is exactly the same: m_distance.
//
// So, for example, a freight terminal might have a row of loading docks with
// LocationId values in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]. The time to travel
// between dock 0 and dock 10 is 10*m_distance time units.
SimTime LinearLocations::transitTimeEstimator(LocationId origin, LocationId destination)
{
    return abs(destination - origin) * m_distance;
}

// Return the next location along the path from the specified origin to the destination.
//
// In the loading dock example, the path from dock 7 to dock 3 first goes
// through dock 6.
LocationId LinearLocations::routeNextStep(LocationId origin, LocationId destination)
{
    if (origin < destination) {
        return origin + 1;
    }
    else if (origin > destination) {
        return origin - 1;
    }
    else {
        return origin;
    }
}


