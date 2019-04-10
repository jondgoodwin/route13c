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
// GraphLocations
//
///////////////////////////////////////////////////////////////////////////////

#include "GraphLocations.h"

namespace Route13Plan
{

    Edge::Edge(LocationId locfrom, LocationId locto, SimTime dist) :
        from(locfrom),
        to(locto),
        distance(dist)
    {
    }


    GraphLocations::GraphLocations(int32_t locs, int32_t loadSpeed, int32_t unloadSpeed) :
        ILocations(locs),
        m_loadSpeed(loadSpeed),
        m_unloadSpeed(unloadSpeed)
    {
    };

    // Add a transit edge to the graph of locations
    void GraphLocations::addEdge(LocationId from, LocationId to, SimTime distance) {
        m_edges.emplace_back(from, to, distance);
    }

    // Pre-calculate transit information, now that all edges have been defined
    void GraphLocations::commit() {

        auto locCount = this->getCount();

        // Set up two-dimensional arrays for distance and next,
        // assuming there is no connectivity to begin with
        for (auto i = 0; i < locCount; ++i) {
            m_distance.emplace_back(locCount, SimTimeMax);
            m_next.emplace_back(locCount, LocationIdNone);
            m_distance[i][i] = 0;
        }

        // Capture initial distance info for every edge
        for (auto edge : m_edges) {
            m_distance[edge.from][edge.to] = edge.distance;
            m_next[edge.from][edge.to] = edge.to;
        }

        // Main loop finds shortest path from `a` to `b` passing through vertex in {0,...k}.
        for (auto k = 0; k < locCount; ++k) {
            for (auto i = 0; i < locCount; ++i) {
                for (auto j = 0; j < locCount; ++j) {
                    auto d = m_distance[i][k] + m_distance[k][j];
                    if (m_distance[i][j] > d) {
                        m_distance[i][j] = d;
                        m_next[i][j] = m_next[i][k];
                    }
                }
            }
        }
    }

    // Return the next location along the path from the specified origin to the destination.
    //
    // In the loading dock example, the path from dock 7 to dock 3 first goes
    // through dock 6.
    LocationId GraphLocations::routeNextStep(LocationId origin, LocationId destination)
    {
        return m_next[origin][destination];
    }

    // The transitTimeEstimator models a sequence of locations along a line.
    // The travel time between two locations is exactly the same: m_distance.
    //
    // So, for example, a freight terminal might have a row of loading docks with
    // LocationId values in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]. The time to travel
    // between dock 0 and dock 10 is 10*m_distance time units.
    SimTime GraphLocations::transitTimeEstimator(LocationId origin, LocationId destination, SimTime time)
    {
        (void)time;
        return m_distance[origin][destination];
    }

    // The loadTimeEstimator models the time to load items onto a cart.
    SimTime GraphLocations::loadTimeEstimator(uint32_t quantity, SimTime time)
    {
        (void)time;
        return m_loadSpeed * quantity;
    }

    // The unloadTimeEstimator models the time to unload items from a cart.
    SimTime GraphLocations::unloadTimeEstimator(int32_t quantity, SimTime time)
    {
        (void)time;
        return m_unloadSpeed * quantity;
    }
}
