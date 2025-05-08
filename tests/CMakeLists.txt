add_executable(ops_test src/ops_test.cpp)
target_link_libraries(ops_test PRIVATE ops gtest_main)