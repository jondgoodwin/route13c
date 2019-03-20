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

#include <cstdlib>
#include <iostream>
#include <vector>
#include "../../src/environment/LinearLocations.h"
#include "../../src/environment/Carts.h"
#include "../../src/environment/Job.h"

using namespace Route13Plan;

#define LOCATION_COUNT 11           // Number of locations in a LinearLocations
#define LOCATION_DISTANCE 100       // Distance between each location in SimTime
#define CART_COUNT 10
#define CART_CAPACITY 10
#define LOAD_SPEED 5
#define UNLOAD_SPEED 2

int main() {
    auto jobCount = 50;
    auto maxStartTime = 1000;
    auto slack = 100;    // Was 2
    //auto maxLookahead = 3;
    //auto startTime = 0;

    // Define the location graph
    auto locations = LinearLocations(LOCATION_COUNT, LOCATION_DISTANCE);

    // Create a fleet of some randomly-located carts
    auto carts = Carts(LOAD_SPEED, UNLOAD_SPEED);
    carts.createRandom(CART_COUNT, CART_CAPACITY, locations.getCount());
    carts.print(std::cout);

    // Create a set of random jobs.
    std::cout << "Creating " << jobCount << " transfer jobs." << std::endl;
    std::vector<IJob*> jobs;
    for (int32_t i = 0; i < jobCount; ++i) {
        auto startLoc = rand() % locations.getCount();
        auto delta = rand() % (locations.getCount() - 2) + 1;  // non-zero distance to end
        auto endLoc = (startLoc + delta) % locations.getCount();  // different than startLoc
        auto quantity = rand() % (CART_CAPACITY - 1) + 1;   // non-zero
        auto startTime = rand() % maxStartTime;

        // End time is based on minimum job duration plus random slack.
        auto minDuration =
            locations.transitTimeEstimator(startLoc, endLoc) +
            carts.loadTimeEstimator(quantity) +
            carts.unloadTimeEstimator(quantity);
        auto endTime = startTime + minDuration * (1 + rand() % slack);

        auto job = new TransferJob(quantity, startLoc, startTime, endLoc, endTime);
        jobs.push_back((IJob*)job);
        std::cout << "    Job " << job->id << " move " << job->quantity
            << " items from " << job->pickupLocation << " to " << job->dropoffLocation
            << " between " << job->pickupAfter << " and " << job->dropoffBefore << std::endl;

    }
    std::cout << std::endl;

}
