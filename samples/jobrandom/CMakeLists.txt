# route13c/samples/jobrandom

set(CPPFILES
    jobrandom.cpp
)

set(PRIVATE_HFILES
)

add_executable(jobrandom jobrandom.cpp)
target_link_libraries(jobrandom Environment Planner)
set_property(TARGET jobrandom PROPERTY FOLDER "samples/jobrandom")
set_property(TARGET jobrandom PROPERTY PROJECT_LABEL "Executable")

