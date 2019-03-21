# route13c

cmake_minimum_required(VERSION 2.6)

# Remove the warning that WIN32 is not defined in Cygwin any more.
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# PRoject-wide settings
project(route13c_CMake)

if(MSVC)
  set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /W4 /WX -D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
  set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_CXX_FLAGS} /MTd")
  set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} ${COMMON_CXX_FLAGS} /MT /Z7")
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /DEBUG" )
elseif(CMAKE_COMPILER_IS_GNUCXX)
  # Need gnu++ instead of c++ so that GTest can access fdopen() etc.
  set(CMAKE_CXX_FLAGS "-msse4.2 -std=gnu++14 -Wall -Wextra -Werror -Wold-style-cast -fstrict-aliasing -Wstrict-aliasing")
else()
  # TODO: define a target for -Weverything.
  # set(CMAKE_CXX_FLAGS "-msse4.2 -std=c++14 -Wall -Wextra -Werror -Wold-style-cast ${WEVERYTHING_FLAGS} -Wno-missing-braces")
  # TODO: define a target for msan.
  # set(CMAKE_CXX_FLAGS "-msse4.2 -std=c++14 -Wall -Wextra -Werror -Wold-style-cast ${MSAN_FLAGS} -Wno-missing-braces")
  set(CMAKE_CXX_FLAGS "-msse4.2 -std=c++14 -Wall -Wextra -Werror -Wold-style-cast -Wno-missing-braces")
endif()


if(LINUX)
    find_package(Threads REQUIRED)
    if(CMAKE_USE_PTHREADS_INIT)
        # TODO: pthreads are an issue in general on linux.
        # TODO: We need a permanent recommendation for when/how to use libc++.
        # On clang for at least some platforms, we require `libc++-dev`.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif()
endif()


# This allows the solution to group projects into folders like src and test.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###############################################################################

set(ENVIRONMENT_HFILES
  ${CMAKE_SOURCE_DIR}/src/environment/Cart.h
  ${CMAKE_SOURCE_DIR}/src/environment/Carts.h
  ${CMAKE_SOURCE_DIR}/src/environment/ILocations.h
  ${CMAKE_SOURCE_DIR}/src/environment/Job.h
  ${CMAKE_SOURCE_DIR}/src/environment/Jobs.h
  ${CMAKE_SOURCE_DIR}/src/environment/LinearLocations.h
  ${CMAKE_SOURCE_DIR}/src/planner/Route.h
  ${CMAKE_SOURCE_DIR}/src/planner/RoutePlanner.h
)

set(PUBLIC_HFILES
  ${ENVIRONMENT_HFILES}
)

# Public header files
add_custom_target(inc SOURCES ${PUBLIC_HFILES})
set_property(TARGET inc PROPERTY FOLDER "")
source_group("environment" FILES ${ENVIRONMENT_HFILES})

#
# Local package include directories.
#
include_directories(
)

#
# Libraries and executables.
#
add_subdirectory(src)
add_subdirectory(samples/jobassigner)
add_subdirectory(samples/routeplanner)

add_custom_target(TOPLEVEL SOURCES
  LICENSE.txt
  README.md
)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
