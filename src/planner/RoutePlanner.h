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

namespace Route13Plan
{

   // RoutePlanner finds the optimal order of pickups and dropoffs, given a
   // starting location and a set of Jobs. The planning algorithm uses a brute
   // force enumeration of every possible route. Routes that don't meet delivery
   // deadline constraints and cart capacity constraints are excluded from
   // consideration. The algorithm selects the route with the shortest elapsed
   // time.
    class RoutePlanner {
    public:

        RoutePlanner(ILocations* locations, Carts* carts, Jobs* jobs);

        void explainRoute();

    private:
        ILocations* m_locations;
        Carts* m_carts;
        Jobs* m_jobs;
    };

}