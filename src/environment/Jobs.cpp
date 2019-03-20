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
// Carts
//
///////////////////////////////////////////////////////////////////////////////

#include "Jobs.h"

namespace Route13Plan
{

    // Add a job to the list of jobs
    void Jobs::addTransfer(int32_t quantity,
                           LocationId pickupLoc,
                           SimTime pickupTime,
                           LocationId dropLoc,
                           SimTime dropBefore)
    {
        jobs.push_back(std::unique_ptr<IJob>(
            new TransferJob(quantity, pickupLoc, pickupTime, dropLoc, dropBefore)));
    }

    // Add an out-of-service job to the list of jobs
    void Jobs::addOutOfService(LocationId suspendLoc,
                         SimTime suspendTime,
                         SimTime resumeTime)
    {
        jobs.push_back(std::unique_ptr<IJob>(
            new OutOfServiceJob(suspendLoc, suspendTime, resumeTime)));
    }

    // Create a random list of job requirements
    void Jobs::createRandom(int32_t count,
                            ILocations* locations,
                            Carts* carts,
                            int32_t capacity,
                            SimTime maxStart,
                            int32_t slack)
    {
        for (int32_t i = 0; i < count; ++i)
        {
            auto startLoc = rand() % locations->getCount();
            auto delta = rand() % (locations->getCount() - 2) + 1;  // non-zero distance to end
            auto endLoc = (startLoc + delta) % locations->getCount();  // different than startLoc
            auto quantity = rand() % (capacity - 1) + 1;   // non-zero
            auto startTime = rand() % maxStart;

            // End time is based on minimum job duration plus random slack.
            auto minDuration =
                locations->transitTimeEstimator(startLoc, endLoc) +
                carts->loadTimeEstimator(quantity) +
                carts->unloadTimeEstimator(quantity);
            auto endTime = startTime + minDuration * (1 + rand() % slack);

            auto job = new TransferJob(quantity, startLoc, startTime, endLoc, endTime);
            jobs.push_back(std::unique_ptr<IJob>(job));
        }
    }

    // Return how many jobs
    size_t Jobs::getCount()
    {
        return jobs.size();
    }

    // Output information about the list of jobs
    void Jobs::print(std::ostream& out)
    {
        out << "We have " << jobs.size() << " jobs:" << std::endl;
        for (auto jobi = jobs.begin(); jobi != jobs.end(); ++jobi)
        {
            auto ijob = jobi->get();
            switch (ijob->type)
            {
            case IJob::JobType::TRANSFER:
            {
                auto tjob = static_cast<TransferJob*>(ijob);
                std::cout << "    Job " << tjob->id << " move " << tjob->quantity
                    << " items from " << tjob->pickupLocation << " to " << tjob->dropoffLocation
                    << " between " << tjob->pickupAfter << " and " << tjob->dropoffBefore << std::endl;
                break;
            }
            case IJob::JobType::OUT_OF_SERVICE:
            {
                auto ojob = static_cast<OutOfServiceJob*>(ijob);
                std::cout << "    Job " << ojob->id << " out-of-service at " << ojob->suspendLocation
                    << " between " << ojob->suspendTime << " and " << ojob->resumeTime << std::endl;
                break;
            }
            }
        }
        out << std::endl;
    }

}