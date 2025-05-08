file(GLOB_RECURSE SRC src/*.cpp)

# ops tests 
add_executable(ops_test ${SRC})
target_include_directories(ops_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ops_test core)

# ops library
add_library(ops ${SRC})
target_include_directories(ops PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ops core)
