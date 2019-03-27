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

#include "Route.h"
#include "RouteState.h"

namespace Route13Plan
{

    Route::Route(Cart* cartp, Jobs* jobs) :
        cart(cartp),
        workingTime(0),
        score(0.0),
        m_actions(jobs)
    {
    }

    void Route::print(std::ostream& out)
    {
        if (score > 0.0) {
            out << "Route for cart " << cart->id << " (working time = " << workingTime
                << " score = " << score << "):" << std::endl;
            for (size_t i = 0; i < m_actionSeq.size(); ++i) {
                out << "    ";
                m_actions.actions[m_actionSeq[i]]->print(out);
            }
            out << std::endl;
        }
        else
            out << "No constraint-free route has been determined." << std::endl;
    }

    bool Route::getBestRoute(ILocations* locations, SimTime time, bool logger)
    {
        int32_t successfulRouteCount = 0;
        m_failedRouteCount = 0;

        // Initialize various working collections
        auto actionCount = m_actions.actions.size();
        std::vector<int32_t> actionSeq(actionCount, 0);  // LIFO sequence of actions
        std::vector<bool> actionInUse(actionCount, false);
        RouteState routeState(cart, time);
        std::vector<RouteState> states(actionCount + 1, routeState); // LIFO queue of states

        if (logger) {
            std::cout << "#########################" << std::endl;
            std::cout << "Find best route" << std::endl << std::endl;
        }

        // Test every valid combination of action sequences
        for (int32_t seq = 0; seq >= 0; )
        {
            auto nextActionId = actionSeq[seq];

            // If we have run out of actions at this level, pop
            if (nextActionId >= actionCount) {
                actionSeq[seq] = 0;
                --seq;
                if (seq >= 0) {
                    actionInUse[actionSeq[seq]] = false;
                    ++actionSeq[seq];
                }
                continue;
            }

            // We won't try an action twice,
            // nor will we try an action that depends on an action not yet performed
            if (actionInUse[nextActionId]
                || (m_actions.actions[nextActionId]->depends != NO_DEPENDENCY &&
                    !actionInUse[m_actions.actions[nextActionId]->depends])) {
                ++actionSeq[seq];
                continue;
            }

            if (logger) {
                std::cout << std::endl << "Try this route of actions: ";
                for (int32_t i = 0; i <= seq; ++i) {
                    std::cout << actionSeq[i] << ", ";
                }
                std::cout << std::endl;
            }

            // Apply action on route state
            // If it violates a constraint this is a failed action sequence
            if (!m_actions.actions[nextActionId]->apply(&states[seq + 1], &states[seq], locations, logger)) {
                ++m_failedRouteCount;
                ++actionSeq[seq];
                continue;
            }

            // If action met constraints but we are not done applying them all, try a follow-on action
            if (seq < actionCount - 1) {
                actionInUse[nextActionId] = true;
                ++seq;
                continue;
            }

            // We have a route that completes all actions and violates no constraints.
            ++successfulRouteCount;
            float successScore = ((float) states[seq + 1].quantityUnloaded) / ((float)states[seq + 1].workingTime);
            if (successScore > score) {
                score = successScore;
                m_actionSeq = actionSeq;
                workingTime = states[seq + 1].workingTime;
            }

            if (logger) {
                std::cout << "    *** Successful route!!!" 
                    << " Working time: " << states[seq + 1].workingTime
                    << " Score: " << successScore << std::endl;
            }

            // Back up and try another combination of actions
            actionSeq[seq] = 0;
            --seq;
            if (seq >= 0) {
                actionInUse[actionSeq[seq]] = false;
                ++actionSeq[seq];
            }
        }

        if (logger) {
            std::cout << "#########################" << std::endl;
            if (score > 0.0) {
                std::cout << "Fastest route selected out of " << successfulRouteCount
                    << " successful routes and " << m_failedRouteCount 
                    << " failed routes considered." << std::endl << std::endl;
                print(std::cout);
            }
            else
                std::cout << "No successful route found after " 
                << m_failedRouteCount << " routes considered." << std::endl;
        }

        return (score > 0.0);
    }
}