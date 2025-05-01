file(GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(utils ${SRC})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

