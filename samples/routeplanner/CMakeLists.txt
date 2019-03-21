# route13c/samples/routeplanner

set(CPPFILES
    routeplanner.cpp
)

set(PRIVATE_HFILES
)

add_executable(routeplanner routeplanner.cpp)
target_link_libraries(routeplanner Environment)
set_property(TARGET routeplanner PROPERTY FOLDER "samples/routeplanner")
set_property(TARGET routeplanner PROPERTY PROJECT_LABEL "Executable")

