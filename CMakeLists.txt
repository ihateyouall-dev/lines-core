cmake_minimum_required(VERSION 3.14)
project(lines LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(lines INTERFACE)

target_include_directories(lines INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_library(Lines::Lines ALIAS lines)

enable_testing()

add_executable(tests
    tests/tasks/task_tests.cpp
    tests/temporal/date_tests.cpp
    tests/temporal/duration_tests.cpp
    tests/temporal/timestamp_tests.cpp
    tests/temporal/timepoint_tests.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest_main Lines::Lines)

add_test(NAME tests COMMAND tests)
