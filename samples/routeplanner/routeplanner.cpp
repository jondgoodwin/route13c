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
#include "../../src/environment/Jobs.h"
#include "../../src/planner/RoutePlanner.h"

using namespace Route13Plan;

#define LOCATION_COUNT 11           // Number of locations in a LinearLocations
#define LOCATION_DISTANCE 100       // Distance between each location in SimTime
#define CART_CAPACITY 10
#define LOAD_SPEED 5
#define UNLOAD_SPEED 2

int main() {

    // Define the location graph
    auto locations = LinearLocations(LOCATION_COUNT, LOCATION_DISTANCE, LOAD_SPEED, UNLOAD_SPEED);

    // Create a fleet with one empty cart starting at location 0
    auto carts = Carts();
    carts.addCart(new Cart(0, 0, CART_CAPACITY, 0));
    carts.print(std::cout);

    // Create a simple set of transfer job requirements
    auto jobs = Jobs();
    jobs.addTransfer(5, 2, 3*60, 10, 30*60);
    jobs.addTransfer(4, 3, 3 * 60, 4, 30 * 60);
    jobs.addOutOfService(9, 30 * 60, 40 * 60);
    jobs.print(std::cout);

    auto planner = new RoutePlanner(&locations, &carts, &jobs);
    planner->explainRoute();
}
