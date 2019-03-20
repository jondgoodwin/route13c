///////////////////////////////////////////////////////////////////////////////
//
// Cart
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include "Location.h"

typedef uint32_t CartId;

// A Cart transports items from one location to another.
class Cart {
public:

    static CartId nextId;

    Cart(LocationId lastKnownLocation, int32_t capacity, int32_t payload = 0);
    Cart() = delete;

    CartId id;

    // Cart capacity. Could be number of boxes/containers, tons, gallons, etc.
    int32_t capacity;

    // Amount of capacity currently in use.
    int32_t payload;

    // Last known location of the cart.
    LocationId lastKnownLocation;


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
