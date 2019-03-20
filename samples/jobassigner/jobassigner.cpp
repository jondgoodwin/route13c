#include <cstdlib>
#include <iostream>
#include <vector>
#include "../../src/environment/LinearLocations.h"
#include "../../src/environment/Cart.h"
#include "../../src/environment/Job.h"

///////////////////////////////////////////////////////////////////////////////
//
// Estimators provided to the simulation.
//
///////////////////////////////////////////////////////////////////////////////

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

#define LOCATION_COUNT 11           // Number of locations in a LinearLocations
#define LOCATION_DISTANCE 100       // Distance between each location in SimTime

int main() {
    auto cartCount = 10;
    auto cartCapacity = 10;
    auto jobCount = 50;
    auto maxStartTime = 1000;
    auto slack = 100;    // Was 2
    //auto maxLookahead = 3;
    //auto startTime = 0;

    // Define the location graph
    auto locations = LinearLocations(LOCATION_COUNT, LOCATION_DISTANCE);

    // Create some randomly-located carts
    std::cout << "Creating " << cartCount << " carts with capacity = " << cartCapacity << "." << std::endl;
    std::vector<Cart> carts;
    for (int32_t i = 0; i < cartCount; ++i)
    {
        Cart cart(rand() % locations.getCount(), cartCapacity, 0);
        carts.push_back(cart);
        std::cout << "     Cart " << cart.id << " at location " << cart.lastKnownLocation 
            << " with capacity " << cart.capacity << "." << std::endl;
    }
    std::cout << std::endl;

    // Create a set of random jobs.
    std::cout << "Creating " << jobCount << " transfer jobs." << std::endl;
    std::vector<IJob*> jobs;
    for (int32_t i = 0; i < jobCount; ++i) {
        auto startLoc = rand() % locations.getCount();
        auto delta = rand() % (locations.getCount() - 2) + 1;  // non-zero distance to end
        auto endLoc = (startLoc + delta) % locations.getCount();  // different than startLoc
        auto quantity = rand() % (cartCapacity - 1) + 1;   // non-zero
        auto startTime = rand() % maxStartTime;

        // End time is based on minimum job duration plus random slack.
        auto minDuration =
            locations.transitTimeEstimator(startLoc, endLoc) +
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


