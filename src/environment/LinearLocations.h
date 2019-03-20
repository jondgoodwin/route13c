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

#pragma once

#include <cstdlib>

#include "ILocations.h"

namespace Route13Plan
{

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

}