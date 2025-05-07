file(GLOB_RECURSE SRC src/*.cpp)
add_library(session ${SRC})
target_include_directories(session {CMAKE_CURRENT_SOURCE_DIR}/include)

