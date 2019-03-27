// The MIT License (MIT)

// Copyright (c) 2019, Microsoft

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////
//
// LinearLocations
//
///////////////////////////////////////////////////////////////////////////////

#include "LinearLocations.h"

namespace Route13Plan
{

    LinearLocations::LinearLocations(int32_t locs, SimTime distance, int32_t loadSpeed, int32_t unloadSpeed) :
        ILocations(locs),
        m_distance(distance),
        m_loadSpeed(loadSpeed),
        m_unloadSpeed(unloadSpeed)
    {
    };

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

    // The transitTimeEstimator models a sequence of locations along a line.
    // The travel time between two locations is exactly the same: m_distance.
    //
    // So, for example, a freight terminal might have a row of loading docks with
    // LocationId values in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]. The time to travel
    // between dock 0 and dock 10 is 10*m_distance time units.
    SimTime LinearLocations::transitTimeEstimator(LocationId origin, LocationId destination, SimTime time)
    {
        (void)time;
        return abs(destination - origin) * m_distance;
    }

    // The loadTimeEstimator models the time to load items onto a cart.
    SimTime LinearLocations::loadTimeEstimator(uint32_t quantity, SimTime time)
    {
        (void)time;
        return m_loadSpeed * quantity;
    }

    // The unloadTimeEstimator models the time to unload items from a cart.
    SimTime LinearLocations::unloadTimeEstimator(int32_t quantity, SimTime time)
    {
        (void)time;
        return m_unloadSpeed * quantity;
    }
}
