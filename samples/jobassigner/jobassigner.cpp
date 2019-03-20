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
    auto locations = LinearLocations(LOCATION_COUNT, LOCATION_DISTANCE);

    // Create a fleet of some randomly-located carts
    auto carts = Carts(LOAD_SPEED, UNLOAD_SPEED);
    carts.createRandom(CART_COUNT, CART_CAPACITY, locations.getCount());
    carts.print(std::cout);

    // Create a set of random jobs.
    auto jobs = Jobs();
    jobs.createRandom(JOB_COUNT, &locations, &carts, CART_CAPACITY, MAX_START_TIME, SLACK);
    jobs.print(std::cout);

}
