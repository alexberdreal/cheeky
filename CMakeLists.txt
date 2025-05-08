cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17) 

project(cheeky)

add_subdirectory(core)
add_subdirectory(loader)
add_subdirectory(ops)
add_subdirectory(session)

add_executable(cheeky main.cpp)

target_link_libraries(cheeky core loader session)
