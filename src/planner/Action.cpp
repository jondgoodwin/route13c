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
// Action
//
///////////////////////////////////////////////////////////////////////////////

#include "Action.h"

namespace Route13Plan
{

    PickupAction::PickupAction(IJob* jobp, LocationId loc, SimTime pickupAfter, int32_t quant) :
        IAction(jobp, ActionType::PICKUP),
        location(loc),
        time(pickupAfter),
        quantity(quant)
    {
    }

    void PickupAction::print(std::ostream& out)
    {
        out << "Pick up " << quantity << " items at location " << location
            << " after " << time << " (job " << job->id << ")." << std::endl;
    }

    DropoffAction::DropoffAction(IJob* jobp, LocationId loc, SimTime dropoffBefore, int32_t quant) :
        IAction(jobp, ActionType::DROPOFF),
        location(loc),
        time(dropoffBefore),
        quantity(quant)
    {
    }

    void DropoffAction::print(std::ostream& out)
    {
        out << "Drop off " << quantity << " items at location " << location
            << " before " << time << " (job " << job->id << ")." << std::endl;
    }

    SuspendAction::SuspendAction(IJob* jobp, LocationId loc, SimTime suspendAfter, SimTime resumeBefore):
        IAction(jobp, ActionType::SUSPEND),
        location(loc),
        suspendTime(suspendAfter),
        resumeTime(resumeBefore)
    {
    }

    void SuspendAction::print(std::ostream& out)
    {
        out << "Suspend at location " << location
            << " between " << suspendTime << " and " << resumeTime 
            << " (job " << job->id << ")." << std::endl;
    }
}