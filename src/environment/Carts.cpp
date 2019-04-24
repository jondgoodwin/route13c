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

#include "Carts.h"

namespace Route13Plan
{

    Carts::Carts()
    {
    }

    // Add a cart to the fleet of carts
    void Carts::addCart(Cart* cart)
    {
        carts.push_back(std::unique_ptr<Cart>(new Cart(cart->id, cart->capacity, cart->availLocation, cart->availTime, cart->availPayload)));
    }

    // Create a random fleet of carts
    void Carts::createRandom(int32_t cartCount, int32_t capacity, int32_t locCount) {
        for (int32_t i = 0; i < cartCount; ++i)
        {
            carts.push_back(std::unique_ptr<Cart>(new Cart(i, capacity, rand() % locCount)));
        }
    }

    // Return how many carts in fleet
    size_t Carts::getCount()
    {
        return carts.size();
    }

    // Output information about the fleet of carts
    void Carts::print(std::ostream& out)
    {
        out << "We have " << carts.size() << " carts:" << std::endl;
        for (auto carti = carts.begin(); carti != carts.end(); ++carti)
        {
            auto &cart = *carti;
            out << "     Cart " << cart->id << " at location " << cart->availLocation
                << " with capacity " << cart->capacity << "." << std::endl;
        }
        out << std::endl;
    }
}