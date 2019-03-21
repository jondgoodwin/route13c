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
// Job
//
///////////////////////////////////////////////////////////////////////////////

#include "Job.h"

namespace Route13Plan
{

    JobId IJob::nextId = 0;

    IJob::IJob(JobType jtype) : id(nextId++), type(jtype), assignedTo(nullptr)
    {
    };

    TransferJob::TransferJob(int32_t number,
                             LocationId pickupLoc,
                             SimTime pickupTime,
                             LocationId dropLoc,
                             SimTime dropBefore) :
        IJob(IJob::TRANSFER),
        state(TransferJob::BEFORE_PICKUP),
        quantity(number),
        pickupLocation(pickupLoc),
        pickupAfter(pickupTime),
        dropoffLocation(dropLoc),
        dropoffBefore(dropBefore)
    {
    }

    OutOfServiceJob::OutOfServiceJob(LocationId suspendLoc,
        SimTime suspendTime,
        SimTime resumeTime) :
        IJob(IJob::OUT_OF_SERVICE),
        state(OutOfServiceJob::BEFORE_BREAK),
        suspendLocation(suspendLoc),
        suspendTime(suspendTime),
        resumeTime(resumeTime)
    {
    }

}