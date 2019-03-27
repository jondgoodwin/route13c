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

    // A Route is an ordered sequence of Actions. Typically, the Actions that
    // make up a Route are the union of the Actions associated with a number of
    // Jobs.
    //
    // A Route represents the interleaving of Actions required to perform a set
    // of Jobs.
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
        Route(Cart* cart, Actions* actions, SimTime time, int32_t score = 0);
        void print(std::ostream&);
        
        Cart* cart;
        SimTime workingTime;
        int32_t score;
        Actions* actions;
    };

}