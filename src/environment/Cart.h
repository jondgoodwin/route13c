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
// Cart
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include "ILocations.h"

typedef uint32_t CartId;

namespace Route13Plan
{

    // A Cart transports items from one location to another.
    //
    // Several information about the cart tells us about the state
    // of the cast when it is next available for further planned actions.
    // This is important if a cart is currently in motion,
    // handling an already tasked assignment.
    class Cart {
    public:

        Cart(int32_t id, int32_t capacity, LocationId availLocation, SimTime time = 0, int32_t payload = 0);
        Cart() = delete;

        CartId id;

        // Cart capacity. Could be number of boxes/containers, tons, gallons, etc.
        int32_t capacity;

        // Location of the cart once it becomes available for further use
        LocationId availLocation;

        // When the cart will be available for follow-work (0 = ready any time)
        SimTime availTime;

        // Amount of capacity in use once cart is available for further use
        int32_t availPayload;

        // DESIGN NOTE: information about jobs currently assigned to a cart is
        // encoded in the Job data structure. Don't want to duplicate this
        // information here to avoid inconsistencies.
        // ISSUE: Cart payload could still be inconsistent with current jobs.
        // We could infer the payload from the jobs. Not sure where this
        // information should reside. There is an argument for not storing the
        // payload quantity with the cart, and that is that we don't anticipate
        // a way to measure this quantity.
        //
        // Could measure the location with GPS tracker.
        // Could measure the payload with RFID, or on/off scans.
    };

}