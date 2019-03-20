///////////////////////////////////////////////////////////////////////////////
//
// Job
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include "ILocations.h"
#include "Cart.h"

typedef uint32_t JobId;

// A Job specifies some requirement that a cart needs to perform
class IJob {
public:

    static JobId nextId;

    enum JobType {
        TRANSFER,
        OUT_OF_SERVICE
    };

    IJob(JobType jtype);
    IJob() = delete;

    JobId id;
    JobType type;
    Cart* assignedTo;
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

    enum TransferJobState {
        BEFORE_PICKUP,
        ENROUTE
    };

    TransferJobState state;

    int32_t quantity;

    LocationId pickupLocation;
    SimTime pickupAfter;

    LocationId dropoffLocation;
    SimTime dropoffBefore;
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

    enum OutOfServiceJobState {
        BEFORE_BREAK,
        ON_BREAK
    };

    OutOfServiceJobState state;
    LocationId suspendLocation;
    SimTime suspendTime;
    SimTime resumeTime;
};
