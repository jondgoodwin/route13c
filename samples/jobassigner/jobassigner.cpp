#include <iostream>
#include <vector>
#include "../../src/environment/Location.h"
#include "../../src/environment/Cart.h"

int main() {
    auto cartCount = 10;
    auto cartCapacity = 10;
    auto locationCount = 11;
    //auto meanCartLoadFactor = 0.5;
    //auto jobCount = 50;
    //auto meanStartTime = 1000;
    //auto slack = 100;    // Was 2
    //auto maxLookahead = 3;
    //auto startTime = 0;

    // Create some randomly-located carts
    std::cout << "Creating " << cartCount << " carts with capacity = " << cartCapacity << "." << std::endl;
    std::vector<Cart> carts;
    for (int32_t i = 0; i < cartCount; ++i)
    {
        Cart cart(i, rand() % locationCount, cartCapacity, 0);
        carts.push_back(cart);
        std::cout << "     Cart " << cart.id << " at location " << cart.lastKnownLocation 
            << " with capacity " << cart.capacity << "." << std::endl;
    }
    std::cout << std::endl;
}
