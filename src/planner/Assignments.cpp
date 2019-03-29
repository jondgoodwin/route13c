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
// Route
//
///////////////////////////////////////////////////////////////////////////////

#include "Assignments.h"

namespace Route13Plan
{

    typedef std::vector<IJob*> JobList;

    Assignments::Assignments(Carts* carts, Jobs *jobs, int32_t maxJobs) :
        m_carts(carts),
        m_jobs(jobs),
        m_maxJobs(maxJobs)
    {
    }

    void Assignments::planBest(ILocations* locations, SimTime time, bool logger)
    {
        // Distribute every job into one of two working vectors:
        // - existingAssignments, if the job has already been assigned to a cart
        // - unassigned, all jobs not yet assigned to a cart
        std::vector<IJob*> unassigned;
        std::vector<std::unique_ptr<JobList>> existingAssignments(m_carts->carts.size());
        for (auto carti = m_carts->carts.begin(); carti != m_carts->carts.end(); ++carti)
        {
            auto cartp = carti->get();
            existingAssignments[cartp->id] = std::unique_ptr<JobList>(new JobList);
        }
        for (auto jobi = m_jobs->jobs.begin(); jobi != m_jobs->jobs.end(); ++jobi)
        {
            auto jobp = jobi->get();
            if (jobp->assignedTo) {
                existingAssignments[jobp->assignedTo->id]->push_back(jobp);
            }
            else {
                unassigned.push_back(jobp);
            }
        }


        (void)locations;
        (void)time;
        (void)logger;
    }
}