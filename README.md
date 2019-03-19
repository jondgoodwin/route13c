# Route13c (C++ Planner)

`Route13` is a framework for building simulators and optimizers for transportation networks. `Route13` includes a number of naive, brute-force and heuristics based optimizers, but its pluggable architecture allows the use of more sophisticated optimizers, such as 
[linear programming solvers](https://en.wikipedia.org/wiki/Linear_programming)
and ML models. `Route13` scenarios include forklifts in warehouses, baggage carts at airports, and trucks on highways. Basically anything that involves workers or equipment moving loads over a network while satisfying constraints around delivery times, equipment capacities, and worker schedules.

For information on how `Route13` works, please see our
[design documents](https://github.com/MikeHopcroft/route13/blob/master/documentation/README.md).

This repository delivers a high-performance tool that supports real-time planning, as part of the overall Route13 vision.
