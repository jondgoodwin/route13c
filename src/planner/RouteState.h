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
// RouteState
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include "../environment/Cart.h"
#include "Actions.h"

namespace Route13Plan
{

    // A RouteState captures the current state of a cart after running
    // some sequence of actions beginning after a certain point in time.
    class RouteState {
    public:
        RouteState(Cart* cart, SimTime time);
        
        // When the simulation began 
        SimTime startTime;

        // The current time after all actions are fully completed
        SimTime time;

        // The current location after performing completed actions
        LocationId location;

        // How many packages are still in the cart after performing completed actions
        int32_t payload;

        // How much time has passed since actions started
        SimTime workingTime;

        // How many packages have been unloaded by all completed actions
        int32_t quantityUnloaded;
    };

}