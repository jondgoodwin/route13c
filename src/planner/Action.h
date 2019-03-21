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
// Action
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include "../environment/Job.h"

namespace Route13Plan
{

    enum ActionType {
        PICKUP,
        DROPOFF,
        SUSPEND
    };

    // Actions are the smallest unit of work. Jobs and Plans consist of ordered
    // sequences of Actions.
    class IAction {
    public:
        IJob* job;
        ActionType type;
    };

    // PickupAction loads a cart with a quantity of items, at a certain location,
    // after a time when the items become available.
    class PickupAction : IAction {
        LocationId location;
        SimTime time;
        int32_t quantity;
    };

    // DropoffAction unloads a quantity of items from a cart, at a certain location,
    // before a certain time.
    class DropoffAction : IAction {
        LocationId location;
        SimTime time;
        int32_t quantity;
    };

    // SuspendAction takes a cart out of service, at a certain location, within a
    // window of time.
    class SuspendAction : IAction {
        LocationId location;
        SimTime suspendTime;
        SimTime resumeTime;
    };

}