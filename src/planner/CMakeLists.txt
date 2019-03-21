# route13c/src/planner/

set(CPPFILES
    RoutePlanner.cpp
)

set(PRIVATE_HFILES
    RoutePlanner.h
)

add_library(Planner ${CPPFILES} ${PRIVATE_HFILES} ${PUBLIC_HFILES})
set_property(TARGET Planner PROPERTY FOLDER "planner")
set_property(TARGET Planner PROPERTY PROJECT_LABEL "planner")
