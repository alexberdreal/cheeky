file(GLOB_RECURSE SRC src/*.cpp)
add_executable(ops_test ${SRC})
target_include_directories(ops_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ops_test utils)