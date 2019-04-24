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
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/environment/LinearLocations.h"
#include "../../src/environment/Carts.h"
#include "../../src/environment/Jobs.h"
#include "../../src/planner/Assignments.h"

using namespace Route13Plan;

#define LOCATION_COUNT 11           // Number of locations in a LinearLocations
#define LOCATION_DISTANCE 100       // Distance between each location in SimTime
#define CART_COUNT 10
#define CART_CAPACITY 10
#define LOAD_SPEED 5
#define UNLOAD_SPEED 2
#define JOB_COUNT 50
#define MAX_START_TIME 1000
#define SLACK 100

int main() {

    // Define the location graph
    auto locations = LinearLocations(LOCATION_COUNT, LOCATION_DISTANCE, LOAD_SPEED, UNLOAD_SPEED);

    // Create a fleet of some randomly-located carts
    auto carts = Carts();
    carts.createRandom(CART_COUNT, CART_CAPACITY, locations.getCount());
    carts.print(std::cout);

    // Create a set of random jobs.
    auto jobs = Jobs();
    jobs.createRandom(JOB_COUNT, &locations, CART_CAPACITY, MAX_START_TIME, SLACK);
    jobs.print(std::cout);

    std::ofstream out("jobrandom.txt");
    out << LOCATION_COUNT << ", " << LOCATION_DISTANCE << ", " << LOAD_SPEED << ", " << UNLOAD_SPEED << std::endl;

    out << CART_COUNT << std::endl;
    for (auto carti = carts.carts.begin(); carti != carts.carts.end(); ++carti) {
        auto cartp = carti->get();
        out << cartp->id << ", " << cartp->capacity << ", " << cartp->availLocation << ", "
            << cartp->availTime << ", " << cartp->availPayload << std::endl;
    }

    out << JOB_COUNT << std::endl;
    for (auto jobi = jobs.jobs.begin(); jobi != jobs.jobs.end(); ++jobi) {
        auto jobp = jobi->get();
        out << jobp->id << ", " << jobp->type << ", ";
        if (jobp->type == JobType::TRANSFER) {
            auto tjobp = (TransferJob *)jobp;
            out << tjobp->quantity << ", "
                << tjobp->pickupLocation << ", " << tjobp->pickupAfter << ", "
                << tjobp->dropoffLocation << ", " << tjobp->dropoffBefore << std::endl;
        }
        else {
            auto sjobp = (OutOfServiceJob *)jobp;
            out << sjobp->suspendLocation << ", " << sjobp->suspendTime << ", "
                << sjobp->resumeTime << std::endl;
        }
    }

}
