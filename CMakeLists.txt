cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17) 

project(cheekydbg)

add_subdirectory(utils)
add_subdirectory(loader)
add_subdirectory(ops)

add_executable(cheeky main.cpp)

target_link_libraries(cheeky utils loader)
