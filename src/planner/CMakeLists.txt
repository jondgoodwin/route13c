# route13c/src/planner/

set(CPPFILES
	Action.cpp
	Actions.cpp
	Assignments.cpp
	Route.cpp
	RouteState.cpp
)

set(PRIVATE_HFILES
	Action.h
	Actions.h
	Assignments.h
	Route.h
	RouteState.h
)

add_library(Planner ${CPPFILES} ${PRIVATE_HFILES} ${PUBLIC_HFILES})
set_property(TARGET Planner PROPERTY FOLDER "planner")
set_property(TARGET Planner PROPERTY PROJECT_LABEL "planner")
