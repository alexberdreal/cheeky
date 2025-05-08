file(GLOB_RECURSE SRC src/*.cpp)

# ops library
add_library(ops ${SRC})
target_include_directories(ops PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ops core)
