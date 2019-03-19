# route13c/src/environment/

set(CPPFILES
    Cart.cpp
)

set(PRIVATE_HFILES
    Cart.h
	Location.h
)

add_library(Environment ${CPPFILES} ${PRIVATE_HFILES} ${PUBLIC_HFILES})
set_property(TARGET Environment PROPERTY FOLDER "environment")
set_property(TARGET Environment PROPERTY PROJECT_LABEL "environment")
