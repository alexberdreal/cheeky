file(GLOB_RECURSE SRC src/*.cpp)
add_executable(ops_test ${SRC})
add_library(ops ${SRC})
target_include_directories(ops_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ops PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ops_test utils)
target_link_libraries(ops utils)

# TODO: ops lib / ops_test bin 
