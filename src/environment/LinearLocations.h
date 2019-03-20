///////////////////////////////////////////////////////////////////////////////
//
// LinearLocations
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdlib>

#include "ILocations.h"

// This models a collection of locations along a line, evenly spaced.
// The distance between two locations is measured in transit time (SimTime).
class LinearLocations : public ILocations {
public:
    LinearLocations(int32_t locations, SimTime distance);

    // Calculate the transit time between two locations
    SimTime transitTimeEstimator(LocationId origin, LocationId destination);

    // Return the next location along the path from the specified origin to the destination.
    LocationId routeNextStep(LocationId origin, LocationId destination);

private:
    SimTime m_distance;
};
