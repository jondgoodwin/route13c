# route13c/src/environment/

set(CPPFILES
    Cart.cpp
	Job.cpp
)

set(PRIVATE_HFILES
    Cart.h
	Job.h
	Location.h
)

add_library(Environment ${CPPFILES} ${PRIVATE_HFILES} ${PUBLIC_HFILES})
set_property(TARGET Environment PROPERTY FOLDER "environment")
set_property(TARGET Environment PROPERTY PROJECT_LABEL "environment")
