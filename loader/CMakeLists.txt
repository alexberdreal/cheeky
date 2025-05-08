file(GLOB_RECURSE SRC src/*.cpp)
add_library(loader ${SRC})
target_include_directories(loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(loader core)

