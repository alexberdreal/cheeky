add_executable(ops_tests src/ops_tests.cpp)
add_executable(exec_tests src/exec_tests.cpp)

target_link_libraries(ops_tests PRIVATE ops gtest_main)
target_link_libraries(exec_tests PRIVATE session gtest_main)
