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
// Job
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <vector>

#include "Cart.h"
#include "ILocations.h"

namespace Route13Plan
{

    typedef uint32_t JobId;

    enum JobType {
        TRANSFER,
        OUT_OF_SERVICE
    };

    // A Job specifies some requirement that a cart needs to perform
    class IJob {
    public:

        static JobId nextId;

        IJob(JobType jtype);
        IJob() = delete;

        JobId id;
        JobType type;
        Cart* assignedTo;
    };

    // A list of jobs
    typedef std::vector<IJob*> JobList;

    enum TransferJobState {
        BEFORE_PICKUP,
        ENROUTE
    };

    // A TransferJob requires a cart to pickup x packages by the pickup time at one location
    // and drop them off at another location before the dropoff time.
    //
    // A valid plan must satisfy the following conditions:
    //   1. Pickup not before pickupAfter time.
    //   2. Sufficient time to load, travel to dropoffLocation, and unload before
    //      dropoffBefore time.
    //   3. Cart capacity not exceeded.
    class TransferJob : public IJob {
    public:
        TransferJob(int32_t quantity,
            LocationId pickupLoc,
            SimTime pickupAfter,
            LocationId dropLoc,
            SimTime dropBefore);
        TransferJob() = delete;

        TransferJobState state;

        int32_t quantity;

        LocationId pickupLocation;
        SimTime pickupAfter;

        LocationId dropoffLocation;
        SimTime dropoffBefore;
    };

    enum OutOfServiceJobState {
        BEFORE_BREAK,
        ON_BREAK
    };

    // An OutOfServiceJob requires a cart to arrive at a specified location
    // by a certain time and stay there for a given period of time.
    // This applies to non-transit requirements: refueling, maintenance, etc.
    //
    // The OutOfServiceJob is mandatory and the scheduler treats it as a
    // constraint. A valid plan must incorporate sufficient transit time
    // to reach the suspendLocation before the suspendTime. Subsequent
    // activity will not be processed until the resumeTime.
    //
    // DESIGN NOTE: Modeling out-of-service as a job, instead of a cart
    // characteristic in order to easily model brief out-of-service periods like
    // refueling. We want the planner to be able to anticipate carts resuming
    // service.
    class OutOfServiceJob : public IJob {
    public:
        OutOfServiceJob(LocationId suspendLoc,
            SimTime suspendTime,
            SimTime resumeTime);
        OutOfServiceJob() = delete;

        OutOfServiceJobState state;
        LocationId suspendLocation;
        SimTime suspendTime;
        SimTime resumeTime;
    };

}