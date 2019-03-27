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

#include <iostream>

#include "Action.h"
#include "RouteState.h"

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

    bool PickupAction::apply(RouteState* newState, RouteState* oldState, ILocations* locations, bool logger)
    {
        if (logger)
        {
            print(std::cout);
        }

        SimTime startTime = oldState->time;

        // Drive cart to desired location, if necessary
        SimTime transitTime = 0;
        if (location != oldState->location)
        {
            transitTime = locations->transitTimeEstimator(oldState->location, location, oldState->time);
            if (logger)
            {
                std::cout << "    " << oldState->time << ": drive for "
                    << transitTime << "s to location " << location << std::endl;
            }
            newState->location = location;
        }
        newState->time = oldState->time + transitTime;

        // Wait until load is available for pickup.
        if (newState->time < time) {
            SimTime waitTime = time - newState->time;
            if (logger)
            {
                std::cout << "    " << newState->time << ": wait " << waitTime
                    << " until " << time << std::endl;
            }
            newState->time = time;
        }

        // Load the packages
        SimTime loadTime = locations->loadTimeEstimator(quantity, newState->time);
        if (logger)
        {
            std::cout << "    " << newState->time << ": load " << quantity
                << " items in " << loadTime << "s." << std::endl;
        }
        newState->time += loadTime;
        newState->payload = oldState->payload + quantity;
        newState->capacity = oldState->capacity;
        if (newState->payload > newState->capacity)
        {
            // This Route is invalid because its payload exceeds cart capacity.
            if (logger)
            {
                std::cout << "    " << newState->time << ": Constraint violated - payload of "
                    << newState->payload << " exceeds capacity of " << newState->capacity << std::endl;
            }
            return false;
        }

        newState->workingTime = oldState->workingTime + (newState->time - startTime);
        return true;
    }

    DropoffAction::DropoffAction(IJob* jobp, LocationId loc, SimTime dropoffBefore, int32_t quant, size_t depends) :
        IAction(jobp, ActionType::DROPOFF, depends),
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

    bool DropoffAction::apply(RouteState* newState, RouteState* oldState, ILocations* locations, bool logger)
    {
        if (logger)
        {
            print(std::cout);
        }

        SimTime startTime = oldState->time;

        // Drive cart to desired location, if necessary
        SimTime transitTime = 0;
        if (location != oldState->location)
        {
            transitTime = locations->transitTimeEstimator(oldState->location, location, oldState->time);
            if (logger)
            {
                std::cout << "    " << oldState->time << ": drive for "
                    << transitTime << "s to location " << location << std::endl;
            }
            newState->location = location;
        }
        newState->time = oldState->time + transitTime;

        // Unload the packages
        SimTime loadTime = locations->unloadTimeEstimator(quantity, newState->time);
        if (logger)
        {
            std::cout << "    " << newState->time << ": unload " << quantity
                << " items in " << loadTime << "s." << std::endl;
        }
        newState->time += loadTime;
        newState->payload = oldState->payload - quantity;
        newState->quantityUnloaded = oldState->quantityUnloaded + quantity;

        // This Route is invalid when drop off happens too late.
        if (newState->time > time)
        {
            if (logger)
            {
                std::cout << "    " << newState->time << ": Constraint violated - drop off after deadline "
                    << time << std::endl;
            }
            return false;
        }

        // Route is invalid if payload is negative (should never happen)
        if (newState->payload < 0)
        {
            if (logger)
            {
                std::cout << "    " << newState->time << ": Constraint violated - negative payload "
                    << time << std::endl;
            }
            return false;
        }

        newState->workingTime = oldState->workingTime + (newState->time - startTime);
        return true;
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

    bool SuspendAction::apply(RouteState* newState, RouteState* oldState, ILocations* locations, bool logger)
    {
        if (logger)
        {
            print(std::cout);
        }

        // Drive cart to desired location, if necessary
        SimTime transitTime = 0;
        if (location != oldState->location)
        {
            transitTime = locations->transitTimeEstimator(oldState->location, location, oldState->time);
            if (logger)
            {
                std::cout << "    " << oldState->time << ": drive for "
                    << transitTime << "s to location " << location << std::endl;
            }
            newState->location = location;
        }
        newState->time = oldState->time + transitTime;
        // NOTE: we do count transit time to suspend location as
        // working time because this time, might have been put to
        // better use transporting items. Would not want a Route
        // that takes a long time driving to the suspend location
        // to score higher than a Route that was delivering items.
        newState->workingTime = newState->workingTime + transitTime;

        // This Route is invalid if cart arrives later than when it is due to go out of service
        if (newState->time > suspendTime)
        {
            if (logger)
            {
                std::cout << "    " << newState->time << ": Constraint violated - suspends after deadline "
                    << suspendTime << std::endl;
            }
            return false;
        }

        if (logger)
        {
            std::cout << "    " << newState->time << ": Suspend operations." << std::endl;
        }

        // Wait until load is available for pickup.
        if (newState->time < resumeTime) {
            SimTime waitTime = resumeTime - newState->time;
            if (logger)
            {
                std::cout << "    " << newState->time << ": wait " << waitTime
                    << " until " << resumeTime << std::endl;
            }
            newState->time = resumeTime;
        }

        if (logger)
        {
            std::cout << "    " << newState->time << ": Resume operations." << std::endl;
        }

        return true;
    }
}