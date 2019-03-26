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
#include <iostream>

#include "../environment/Job.h"

namespace Route13Plan
{
    class RouteState;

    // What sort of action this is:  pick up, drop off, suspend
    enum ActionType {
        PICKUP,
        DROPOFF,
        SUSPEND
    };

    #define NO_DEPENDENCY -1

    // Actions are the smallest unit of work. Jobs and Plans consist of ordered
    // sequences of Actions.
    class IAction {
    public:
        IAction(IJob* jobp, ActionType typ, size_t dep = NO_DEPENDENCY) : job(jobp), type(typ), depends(dep) {};
        virtual void print(std::ostream&) = 0;
        virtual bool apply(RouteState* newState, RouteState* oldState, ILocations* locations, bool logger) = 0;

        // The job this action was generated from
        IJob* job;

        // What sort of action this is:  pick up, drop off, suspend
        ActionType type;

        // References another action which must be applied before this one in a route sequence
        // (e.g., a drop off action will depend on an earlier pick up action)
        // It is NO_DEPENDENCY if there are no dependencies
        size_t depends;
    };

    // PickupAction loads a cart with a quantity of items, at a certain location,
    // after a time when the items become available.
    class PickupAction : public IAction {
    public:
        PickupAction(IJob* job, LocationId loc, SimTime pickupAfter, int32_t quant);
        void print(std::ostream&);
        // Apply pick up action to calculate new RouteState from old
        // Return false if constraints violated. 
        bool apply(RouteState* newState, RouteState* oldState, ILocations* locations, bool logger);

        LocationId location;
        SimTime time;
        int32_t quantity;
    };

    // DropoffAction unloads a quantity of items from a cart, at a certain location,
    // before a certain time.
    class DropoffAction : public IAction {
    public:
        DropoffAction(IJob* job, LocationId loc, SimTime dropoffBefore, int32_t quant, size_t depends);
        void print(std::ostream&);
        // Apply pick up action to calculate new RouteState from old
        // Return false if constraints violated. 
        bool apply(RouteState* newState, RouteState* oldState, ILocations* locations, bool logger);

        LocationId location;
        SimTime time;
        int32_t quantity;
    };

    // SuspendAction takes a cart out of service, at a certain location, within a
    // window of time.
    class SuspendAction : public IAction {
    public:
        SuspendAction(IJob* job, LocationId loc, SimTime suspendAfter, SimTime resumeBefore);
        void print(std::ostream&);
        // Apply pick up action to calculate new RouteState from old
        // Return false if constraints violated. 
        bool apply(RouteState* newState, RouteState* oldState, ILocations* locations, bool logger);

        LocationId location;
        SimTime suspendTime;
        SimTime resumeTime;
    };

}