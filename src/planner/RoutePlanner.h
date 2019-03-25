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
// RoutePlanner
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include "../environment/Jobs.h"
#include "../planner/Route.h"

namespace Route13Plan
{

    // RoutePlanner finds the optimal order of pickups and dropoffs, given a
    // cart and a set of Jobs. The planning algorithm uses a brute
    // force enumeration of every possible route. Routes that don't meet delivery
    // deadline constraints and cart capacity constraints are excluded from
    // consideration. The algorithm selects the route with the shortest elapsed
    // time.
    class RoutePlanner {
    public:

        RoutePlanner(ILocations* locations, int32_t maxJobs, bool logger);

        // Finds the shortest duration Route for a set of jobs, while satisfying
        // the following constraints:
        //    1. The first Action associated with each Job must appear before its
        //       corresponding second Action.
        //    2. The cart capacity is never exceeded.
        //    3. Pickups happen after loads become available.
        //    4. Dropoffs happen before deadlines.
        //
        // cart
        //   The cart that will perform this job. Cart specifies its capacity along
        //   with for initial location and payload.
        // jobs
        //   An array of jobs the cart should perform. The number of jobs should
        //   not exceed the `maxJobs` limit provided to the constructor.
        // time
        //   The time at which the route should start. This value is passed to the
        //   estimator functions. This allows the estimator functions to model
        //   effects like rush hour conjestion.
        //
        // Returns the Route with the shortest working time that satisfies the
        // constraints. Working time is defined as time that the cart is moving
        // between locations, waiting to load, loading, and unloading. It does
        // not include time when the cart is out of service.
        //
        // Returns null if no Route satisfies the constraints.
        Route* getBestRoute(Cart* cart, Jobs* jobs, SimTime time);

        void explainRoute();

    private:
        ILocations* m_locations;
        int32_t m_maxJobs;
        bool m_logger;
        int32_t m_failedRouteCount;
    };

}