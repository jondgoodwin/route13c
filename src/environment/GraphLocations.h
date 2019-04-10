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

#pragma once

#include <cstdlib>
#include <vector>

#include "ILocations.h"

namespace Route13Plan
{
    // A valid route between two locations, whose distance
    // represents the travel time
    class Edge {
    public:
        Edge(LocationId from, LocationId to, SimTime distance);
        LocationId from;
        LocationId to;
        SimTime distance;
    };


    // This models a collection of edges describing travel routes between
    // all locations.
    // The distance between two locations is measured in transit time (SimTime).
    //
    // It implements estimators that determine how long it takes
    // for work to get accomplished, including cart transit times and
    // package loading and unloading.
    class GraphLocations : public ILocations {
    public:
        GraphLocations(int32_t locations, int32_t loadSpeed, int32_t unloadSpeed);

        // Add a transit edge to the graph of locations
        void addEdge(LocationId from, LocationId to, SimTime distance);

        // Pre-calculate transit information, now that all edges have been defined
        void commit();

        // Return the next location along the path from the specified origin to the destination.
        LocationId routeNextStep(LocationId origin, LocationId destination);

        // Calculate the transit time between two locations
        // We want time in case congestion affects the estimate
        SimTime transitTimeEstimator(LocationId origin, LocationId destination, SimTime time);

        // The loadTimeEstimator models the time to load items onto a cart.
        // We want time in case congestion affects the estimate
        SimTime loadTimeEstimator(uint32_t quantity, SimTime time);

        // The unloadTimeEstimator models the time to unload items from a cart.
        // We want time in case congestion affects the estimate
        SimTime unloadTimeEstimator(int32_t quantity, SimTime time);

    private:
        int32_t m_loadSpeed;
        int32_t m_unloadSpeed;
        std::vector<Edge> m_edges;
        std::vector<std::vector<SimTime>> m_distance;
        std::vector<std::vector<LocationId>> m_next;
    };
}