# route13c/src/planner/

set(CPPFILES
	Action.cpp
	Actions.cpp
    RoutePlanner.cpp
)

set(PRIVATE_HFILES
	Action.h
	Actions.h
	Route.h
    RoutePlanner.h
)

add_library(Planner ${CPPFILES} ${PRIVATE_HFILES} ${PUBLIC_HFILES})
set_property(TARGET Planner PROPERTY FOLDER "planner")
set_property(TARGET Planner PROPERTY PROJECT_LABEL "planner")
