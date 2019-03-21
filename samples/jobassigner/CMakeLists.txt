# route13c/samples/jobassigner

set(CPPFILES
    jobanalyzer.cpp
)

set(PRIVATE_HFILES
)

add_executable(jobassigner jobassigner.cpp)
target_link_libraries(jobassigner Environment Planner)
set_property(TARGET jobassigner PROPERTY FOLDER "samples/jobassigner")
set_property(TARGET jobassigner PROPERTY PROJECT_LABEL "Executable")

