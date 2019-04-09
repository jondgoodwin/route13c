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

#include <algorithm>
#include <limits>

#include "Assignments.h"

namespace Route13Plan
{

    Assignment::Assignment(int32_t cart, JobList *jobs, float scor) :
        cartid(cart),
        score(scor)
    {
        for (auto jobp : *jobs) {
            joblist.push_back(jobp);
        }
    }

    Assignments::Assignments(Carts* carts, Jobs *jobs, int32_t maxJobs) :
        m_carts(carts),
        m_jobs(jobs),
        m_maxJobs(maxJobs)
    {
    }

    bool assigncmp(Assignment a, Assignment b) {
        return a.score > b.score;
    }

    void Assignments::planBest(ILocations* locations, SimTime time, bool logger)
    {
        // Save in case we need for print
        m_locations = locations;
        m_time = time;

        // Distribute every job into one of two working lists:
        // - existingAssignments, if the job has already been assigned to a cart
        // - unassigned, all jobs not yet assigned to a cart
        //
        // Warning: For now, the code assumes that cart ids go from 0 .. carts.size()
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

        // Build a collection of viable job assignments for every cart
        std::vector<Assignment> alternatives;
        std::vector<int32_t> jobSeq(m_maxJobs, 0);  // LIFO sequence of jobs
        for (auto cartid = 0; cartid < existingAssignments.size(); ++cartid) {
            Cart* cartp = m_carts->carts[cartid].get();
            JobList* joblist = existingAssignments[cartid].get();

            // If a cart has enough pre-assigned jobs, this is the cart's only plan
            // Set score to maximum, so it gets selected first
            if (joblist->size() >= m_maxJobs) {
                alternatives.emplace_back(cartid, joblist, std::numeric_limits<float>::infinity());
                continue;
            }

            // Try every possible combination of up to m_maxJobs jobs on this cart
            auto maxJobCount = m_maxJobs - joblist->size();
            for (int32_t seq = 0; seq >= 0; ) {
                auto nextJobId = jobSeq[seq];

                // If we have run out of jobs at this level, pop
                if (nextJobId >= m_jobs->jobs.size()) {
                    jobSeq[seq] = 0;
                    --seq;
                    if (seq >= 0) {
                        joblist->pop_back();
                        ++jobSeq[seq];
                    }
                    continue;
                }

                // Don't try job if already assigned to a cart
                IJob* jobp = m_jobs->jobs[nextJobId].get();
                if (jobp->assignedTo) {
                    ++jobSeq[seq];
                    continue;
                }

                // Don't try job if we already have it in the list
                for (auto assignedJob : *joblist) {
                    if (jobp == assignedJob) {
                        jobp = nullptr;
                        break;
                    }
                }

                // Add any valid job to list, then find/save best viable route
                if (jobp) {
                    joblist->push_back(jobp);
                    Route route(cartp, joblist);
                    if (route.getBestRoute(locations, time, false)) {
                        alternatives.emplace_back(cartid, joblist, route.score);
                        if (seq < maxJobCount - 1) {
                            ++seq;
                            continue;
                        }
                    }
                    joblist->pop_back();
                }
                ++jobSeq[seq];
            }
        }

        // Sort all assignments by descending score
        std::sort(alternatives.begin(), alternatives.end(), assigncmp);

        assignments.reserve(m_carts->carts.size());
        std::vector<bool> isCartAssigned(m_carts->carts.size(), false);
        std::vector<bool> isJobAssigned(m_jobs->jobs.size(), false);
        for (auto assignment : alternatives) {
            bool conflicting = isCartAssigned[assignment.cartid];
            if (!conflicting) {
                for (auto job : assignment.joblist) {
                    if (isJobAssigned[job->id]) {
                        conflicting = true;
                        break;
                    }
                }
            }
            if (!conflicting) {
                assignments.emplace_back(assignment.cartid, &assignment.joblist, assignment.score);
                isCartAssigned[assignment.cartid] = true;
                for (auto job : assignment.joblist) {
                    isJobAssigned[job->id] = true;
                }
            }
        }

        if (logger) {
            std::cout << "After scoring " << alternatives.size() << " valid alternative routes:"
                << std::endl << std::endl;
            print(std::cout);
        }
    }

    void Assignments::print(std::ostream& out) {
        for (auto assignment : assignments) {
            Route route(m_carts->carts[assignment.cartid].get(), &assignment.joblist);
            route.getBestRoute(m_locations, m_time, false);
            route.print(out);
        }
    }
}