cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17) 

project(cheeky)

# Subdirectories
add_subdirectory(core)
add_subdirectory(loader)
add_subdirectory(ops)
add_subdirectory(session)

if (${BUILDTESTS}) 
    # Fetch GTest library
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
    )
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(tests)
endif()

# Main cheeky executable
add_executable(cheeky main.cpp)
target_link_libraries(cheeky core loader session)
