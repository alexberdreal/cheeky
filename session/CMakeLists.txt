file(GLOB_RECURSE SRC src/*.cpp)
add_library(session ${SRC})
target_include_directories(session PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(session ops loader)