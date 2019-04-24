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

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../../src/environment/GraphLocations.h"
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

void run(Carts *carts, Jobs *jobs, ILocations *locations, int maxLookAhead) {
    clock_t time = clock();
    Assignments assignments(carts, jobs, maxLookAhead);
    assignments.planBest(locations, 0, false);
    printf("Job Assignment took %f seconds.\n\n", ((float)clock() - time) / CLOCKS_PER_SEC);
    assignments.print(std::cout);
}

void randomRun(int maxLookAhead) {
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
    run(&carts, &jobs, &locations, maxLookAhead);
}

int fileRun(int maxLookAhead) {
    std::ifstream file("jobrandom.txt");
    if (!file.is_open())
        return 0;

    // Read and set up graph-based locations
    std::string line;
    std::getline(file, line);
    int locCount, edgeCount, loadSpeed, unloadSpeed;
    sscanf(line.c_str(), "%d, %d, %d, %d", &locCount, &edgeCount, &loadSpeed, &unloadSpeed);
    auto locations = GraphLocations(locCount, loadSpeed, unloadSpeed);
    while (edgeCount--) {
        std::getline(file, line);
        int from, to, dist;
        sscanf(line.c_str(), "%d, %d, %d", &from, &to, &dist);
        locations.addEdge(from, to, dist);
    }
    locations.commit();

    // Read and set up carts
    auto carts = Carts();
    std::getline(file, line);
    int cartCount;
    sscanf(line.c_str(), "%d", &cartCount);
    while (cartCount--) {
        std::getline(file, line);
        int cartId, cartCapacity, cartTime, cartLoc, cartPayload;
        sscanf(line.c_str(), "%d, %d, %d, %d, %d", &cartId, &cartCapacity, &cartTime, &cartLoc, &cartPayload);
        Cart cart = Cart(cartId, cartCapacity, cartLoc, cartTime, cartPayload);
        carts.addCart(&cart);
    }
    carts.print(std::cout);

    // Read and set up carts
    auto jobs = Jobs();
    std::getline(file, line);
    int jobCount;
    sscanf(line.c_str(), "%d", &jobCount);
    while (jobCount--) {
        std::getline(file, line);
        int jobid, jobtype, jobqty, jobPickLoc, jobPickAfter, jobDropLoc, jobDropBefore;
        sscanf(line.c_str(), "%d, %d", &jobid, &jobtype);
        if (jobtype == 0) {
            sscanf(line.c_str(), "%d, %d, %d, %d, %d, %d, %d", &jobid, &jobtype, &jobqty, &jobPickLoc, &jobPickAfter,
                &jobDropLoc, &jobDropBefore);
            jobs.addTransfer(jobqty, jobPickLoc, jobPickAfter, jobDropLoc, jobDropBefore);
        }
        else {
            int suspendLoc, suspendTime, resumeTime;
            sscanf(line.c_str(), "%d, %d, %d, %d, %d", &jobid, &jobtype, &suspendLoc, &suspendTime, &resumeTime);
            jobs.addOutOfService(suspendLoc, suspendTime, resumeTime);
        }
    }
    jobs.print(std::cout);

    run(&carts, &jobs, &locations, maxLookAhead);
    return 1;
}

int main(int argc, char **argv) {

    int maxLookAhead = 3;
    if (argc > 1) {
        maxLookAhead = atoi(argv[1]);
    }

    if (!fileRun(maxLookAhead)) {
        randomRun(maxLookAhead);
    }
}

