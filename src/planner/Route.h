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
// Route
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include "../environment/Cart.h"
#include "Actions.h"

namespace Route13Plan
{

    // A Route is an ordered sequence of Actions, derived from a set of jobs.
    // During construction, there is no guarantee that the route won't violate
    // constraints. Once the best route is calculated, the resulting route represents
    // the best sequence of actions that doesn't violate any constraints and
    // which has the best score. If score is 0, we don't have a valid route.
    //
    // Without the concept of a Route, it would not be possible to interleave the
    // Actions of multiple Jobs. This would preclude opportunities like picking
    // up items for Job B, while tranporting items for Job A, e.g.
    //
    //   Job A: pickup 5 at location 0
    //   Job B: pickup 7 at location 3
    //   Job A: dropoff 5 at location 4
    //   Job B: dropoff 7 at location 5.
    class Route {
    public:
        // Initialize information about a route performed
        // by a specified cart performing all specified jobs.
        Route(Cart* cart, JobList* jobs);

        // Output information about the route
        void print(std::ostream&);
        
        // Find the optimal order of pickups, dropoffs, and suspends given a
        // cart, a set of actions and starting time. The planning algorithm uses a brute
        // force enumeration of every possible route.
        //
        // The best route must satisfy these constraints:
        //    1. The first Action associated with each Job must appear before its
        //       corresponding second Action.
        //    2. The cart capacity is never exceeded.
        //    3. Pickups happen after loads become available.
        //    4. Dropoffs happen before deadlines.
        //
        // Returns true if a quickest, acceptable route was found.
        // The best route has the highest quantity unloaded per working time.
        // Working time is defined as time that the cart is moving
        // between locations, waiting to load, loading, and unloading. It does
        // not include time when the cart is out of service.
        bool getBestRoute(ILocations* locations, SimTime time, bool logger);

        Cart* cart;
        SimTime workingTime;
        float score;

    private:
        Actions m_actions;
        std::vector<int32_t> m_actionSeq;
        int32_t m_failedRouteCount;

    };

}