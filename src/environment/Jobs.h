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
// Jobs
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <iostream>

#include "Carts.h"
#include "ILocations.h"
#include "Job.h"

namespace Route13Plan
{

    // A collection of job requirements
    class Jobs {
    public:
        // Add a transfer job to the list of jobs
        void addTransfer(int32_t quantity,
                         LocationId pickupLoc,
                         SimTime pickupTime,
                         LocationId dropLoc,
                         SimTime dropBefore);

        // Add an out-of-service job to the list of jobs
        void addOutOfService(LocationId suspendLoc,
                             SimTime suspendTime,
                             SimTime resumeTime);

        // Create a random list of jobs
        void createRandom(int32_t count,
                          ILocations* locations,
                          Carts* carts,
                          int32_t capacity,
                          SimTime maxStart,
                          int32_t slack);

        // Count of jobs
        size_t getCount();

        // Output to stream information about the jobs
        void print(std::ostream& out);

        std::vector<std::unique_ptr<IJob>> jobs;
    };

}