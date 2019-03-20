#include <cstdlib>
#include <iostream>
#include <vector>
#include "../../src/environment/Location.h"
#include "../../src/environment/Cart.h"
#include "../../src/environment/Job.h"

///////////////////////////////////////////////////////////////////////////////
//
// Estimators provided to the simulation.
//
///////////////////////////////////////////////////////////////////////////////

// The transitTimeEstimator models a sequence of locations along a line.
// The travel time between two locations is just 100 time units per unit
// distance, where distance is expressed as the difference between LocationId
// values.
//
// So, for example, a freight terminal might have a row of loading docks with
// LocationId values in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]. The time to travel
// between dock 0 and dock 10 is 1000 time units.
//
// NOTE: this function can be generated from a directed graph.
SimTime transitTimeEstimator(LocationId origin, LocationId destination, SimTime startTime)
{
    (void)startTime;
    return abs(destination - origin) * 100;
}

// This function gives the next location along the path from the specified
// origin to the destination.
//
// In the loading dock example, the path from dock 7 to dock 3 first goes
// through dock 6.
//
// NOTE: this function can be generated from a directed graph.
LocationId routeNextStep(LocationId origin, LocationId destination) 
{
    if (origin < destination) {
        return origin + 1;
    }
    else if (origin > destination) {
        return origin - 1;
    }
    else {
        return origin;
    }
}

// The loadTimeEstimator models the time to load items onto a cart. Loading an
// item takes 5 units of time. 
SimTime loadTimeEstimator(LocationId location, uint32_t quantity, SimTime startTime)
{
    (void)location;
    (void)startTime;
    return 5 * quantity;
}

// The unloadTimeEstimator models the time to unload items from a cart.
// Unloading an item takes 2 units of time. 
SimTime unloadTimeEstimator(LocationId location, int32_t quantity, SimTime startTime)
{
    (void)location;
    (void)startTime;
    return 2 * quantity;
}

int main() {
    auto locationCount = 11;
    auto cartCount = 10;
    auto cartCapacity = 10;
    auto jobCount = 50;
    auto maxStartTime = 1000;
    auto slack = 100;    // Was 2
    //auto maxLookahead = 3;
    //auto startTime = 0;

    // Create some randomly-located carts
    std::cout << "Creating " << cartCount << " carts with capacity = " << cartCapacity << "." << std::endl;
    std::vector<Cart> carts;
    for (int32_t i = 0; i < cartCount; ++i)
    {
        Cart cart(rand() % locationCount, cartCapacity, 0);
        carts.push_back(cart);
        std::cout << "     Cart " << cart.id << " at location " << cart.lastKnownLocation 
            << " with capacity " << cart.capacity << "." << std::endl;
    }
    std::cout << std::endl;

    // Create a set of random jobs.
    std::cout << "Creating " << jobCount << " transfer jobs." << std::endl;
    std::vector<IJob*> jobs;
    for (int32_t i = 0; i < jobCount; ++i) {
        auto startLoc = rand() % locationCount;
        auto delta = rand() % (locationCount - 2) + 1;  // non-zero distance to end
        auto endLoc = (startLoc + delta) % locationCount;  // different than startLoc
        auto quantity = rand() % (cartCapacity - 1) + 1;   // non-zero
        auto startTime = rand() % maxStartTime;

        // End time is based on minimum job duration plus random slack.
        auto minDuration =
            transitTimeEstimator(startLoc, endLoc, startTime) +
            loadTimeEstimator(startLoc, quantity, startTime) +
            unloadTimeEstimator(endLoc, quantity, startTime);
        auto endTime = startTime + minDuration * (1 + rand() % slack);

        auto job = new TransferJob(quantity, startLoc, startTime, endLoc, endTime);
        jobs.push_back((IJob*)job);
        std::cout << "    Job " << job->id << " move " << job->quantity
            << " items from " << job->pickupLocation << " to " << job->dropoffLocation
            << " between " << job->pickupAfter << " and " << job->dropoffBefore << std::endl;

    }
    std::cout << std::endl;

}


