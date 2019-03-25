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

#include "RoutePlanner.h"
#include "RouteState.h"

namespace Route13Plan
{

    RoutePlanner::RoutePlanner(ILocations* locations, int32_t maxJobs, bool logger) :
        m_locations(locations),
        m_maxJobs(maxJobs),
        m_logger(logger),
        m_failedRouteCount(0)
    {
    }

    Route* RoutePlanner::getBestRoute(Cart* cart, Jobs* jobs, SimTime time)
    {
        int32_t successfulRouteCount = 0;
        m_failedRouteCount = 0;

        auto routeState = new RouteState(cart, time);
        auto actions = new Actions(jobs);
        bool success = false;
        for (auto actioni = actions->actions.begin(); actioni != actions->actions.end(); ++actioni)
        {
            auto actionp = actioni->get();
            success = actionp->apply(routeState, routeState, m_locations, m_logger);
            if (!success)
            {
                ++m_failedRouteCount;
                break;
            }
        }
        if (success)
            ++successfulRouteCount;

        return NULL;
    }

    void RoutePlanner::explainRoute()
    {

    }
}