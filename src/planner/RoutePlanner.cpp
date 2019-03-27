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
// RoutePlanner
//
///////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "RoutePlanner.h"
#include "RouteState.h"

namespace Route13Plan
{

    RoutePlanner::RoutePlanner(Cart* cart, Jobs* jobs, SimTime time) :
        m_cart(cart),
        m_jobs(jobs),
        m_time(time),
        m_actions(jobs),            // Turn jobs into finer-grained actions
        m_failedRouteCount(0)
    {
    }

    Route* RoutePlanner::getBestRoute(ILocations* locations, bool logger)
    {
        int32_t successfulRouteCount = 0;
        m_failedRouteCount = 0;

        // Initialize various working collections
        auto actionCount = m_actions.actions.size();
        std::vector<int32_t> actionSeq(actionCount, 0);  // LIFO sequence of actions
        std::vector<bool> actionInUse(actionCount, false);
        RouteState routeState(m_cart, m_time);
        std::vector<RouteState> states(actionCount + 1, routeState); // LIFO queue of states

        // Test every valid combination of action sequences
        for (int32_t seq = 0; seq >= 0; )
        {
            auto nextActionId = actionSeq[seq];

            // If we have run out of actions at this level, pop
            if (nextActionId >= actionCount) {
                actionSeq[seq] = 0;
                --seq;
                if (seq >= 0)
                    actionInUse[actionSeq[seq]] = false;
            }

            // We won't try an action twice,
            // nor will we try an action that depends on an action not yet performed
            else if (actionInUse[nextActionId]
                 || (m_actions.actions[nextActionId]->depends != NO_DEPENDENCY &&
                    !actionInUse[m_actions.actions[nextActionId]->depends])) {
            }

            // Apply action on route state, if it violates a constraint this is a failed action sequence
            else if (!m_actions.actions[nextActionId]->apply(&states[seq + 1], &states[seq], locations, logger)) {
                ++m_failedRouteCount;
            }
            
            // If action met constraints, keep going to try a follow-on action
            else if (seq < actionCount - 1) {
                actionInUse[nextActionId] = true;
                ++seq;
                continue;
            }

            // We have a route that completes all actions and violates no constraints.
            // share the good news, then back up and try another combination of actions.
            else {
                ++successfulRouteCount;
                if (logger) {
                    std::cout << "Successful route: ";
                    for (int32_t i = 0; i <= seq; ++i) {
                        std::cout << actionSeq[i] << ", ";
                    }
                    std::cout << std::endl << std::endl;
                }

                actionSeq[seq] = 0;
                --seq;
                actionInUse[actionSeq[seq]] = false;
            }

            if (seq >= 0) {
                ++actionSeq[seq];
            }
        }

        return NULL;
    }

    void RoutePlanner::explainRoute()
    {

    }
}