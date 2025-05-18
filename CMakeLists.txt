cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17) 

project(cheeky)

add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Subdirectories
add_subdirectory(core)
add_subdirectory(loader)
add_subdirectory(ops)
add_subdirectory(session)

include(FetchContent)

# Main cheeky executable
add_executable(cheeky main.cpp)
target_link_libraries(cheeky session)

if (LOGLEVEL STREQUAL "OFF")
    set(SPDLOG_LEVEL_MACRO "SPDLOG_LEVEL_OFF")
elseif(LOGLEVEL STREQUAL "DEBUG")
    set(SPDLOG_LEVEL_MACRO "SPDLOG_LEVEL_DEBUG")
elseif(LOGLEVEL STREQUAL "INFO")
    set(SPDLOG_LEVEL_MACRO "SPDLOG_LEVEL_INFO")
elseif(LOGLEVEL STREQUAL "WARN")
    set(SPDLOG_LEVEL_MACRO "SPDLOG_LEVEL_WARN")
elseif(LOGLEVEL STREQUAL "ERROR")
    set(SPDLOG_LEVEL_MACRO "SPDLOG_LEVEL_ERROR")
else()
    message(WARNING "Unknown LOGLEVEL '${LOGLEVEL}', defaulting to INFO")
    set(SPDLOG_LEVEL_MACRO "SPDLOG_LEVEL_INFO")
endif()

add_compile_definitions(SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_MACRO)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.11.0
)
FetchContent_MakeAvailable(spdlog)

if (${BUILDTESTS}) 
    # Fetch GTest library
    
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
    )
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(tests)
endif()