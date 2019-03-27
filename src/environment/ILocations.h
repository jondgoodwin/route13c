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
// ILocations
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

namespace Route13Plan
{

    typedef int32_t LocationId;

    typedef uint32_t SimTime;

    // ILocations models the "field" on which carts travel to get work down.
    // It is useful for determining best routes.
    //
    // Importantly, it implements estimators that determine how long it takes
    // for work to get accomplished, including cart transit times and
    // package loading and unloading.
    class ILocations {
    public:
        ILocations(int32_t count) : m_count(count) {};

        int32_t getCount() { return m_count; }

        // Return the next location along the path from the specified origin to the destination.
        virtual LocationId routeNextStep(LocationId origin, LocationId destination) = 0;

        // Calculate the transit time between two locations
        // We want time in case congestion affects the estimate
        virtual SimTime transitTimeEstimator(LocationId origin, LocationId destination, SimTime time) = 0;

        // The loadTimeEstimator models the time to load items onto a cart.
        // We want time in case congestion affects the estimate
        virtual SimTime loadTimeEstimator(uint32_t quantity, SimTime time) = 0;

        // The unloadTimeEstimator models the time to unload items from a cart.
        // We want time in case congestion affects the estimate
        virtual SimTime unloadTimeEstimator(int32_t quantity, SimTime time) = 0;

    private:
        int32_t m_count;
    };

}
