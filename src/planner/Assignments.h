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
// Assignments
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include "Route.h"

namespace Route13Plan
{

    // An assignment describes a specific, scored list of jobs that
    // a specific cart can successfully perform without violating any constraint
    class Assignment {
    public:
        Assignment(int32_t cartid, JobList *jobs, float score);

        int32_t cartid;
        JobList joblist;
        float score;
    };

    // Assignments lists the jobs that have been assigned to every available cart.
    //
    // This is determined by evaluating all combinations of jobs for
    // every cart, and brute-force selecting those with the best score.
    class Assignments {
    public:
        Assignments(Carts* carts, Jobs *jobs, int32_t maxJobs);

        // Brute force enumeration of all carts paired with all combinations of N jobs.
        // Then greedy selection from enumerated job assignments.
        void planBest(ILocations* locations, SimTime time, bool logger);

        void print(std::ostream& out);

        // List of assignments by cart
        std::vector<Assignment> assignments;

    private:
        Carts* m_carts;
        Jobs* m_jobs;
        int32_t m_maxJobs;
        ILocations* m_locations;
        SimTime m_time;
    };
}