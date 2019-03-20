# route13c/src/environment/

set(CPPFILES
    Cart.cpp
	Carts.cpp
	Job.cpp
	Jobs.cpp
	LinearLocations.cpp
)

set(PRIVATE_HFILES
    Cart.h
	Carts.h
	ILocations.h
	Job.h
	Jobs.h
	LinearLocations.h
)

add_library(Environment ${CPPFILES} ${PRIVATE_HFILES} ${PUBLIC_HFILES})
set_property(TARGET Environment PROPERTY FOLDER "environment")
set_property(TARGET Environment PROPERTY PROJECT_LABEL "environment")
