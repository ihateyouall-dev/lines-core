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
add_library(temporal)
add_library(tasks)

target_sources(temporal
    PRIVATE
    src/temporal/timestamp.cpp
    src/temporal/timepoint.cpp
    src/temporal/ymd.cpp
    src/temporal/date.cpp
)

target_sources(tasks
    PRIVATE
    src/tasks/task.cpp
    src/tasks/task_completion.cpp
)

target_include_directories(temporal
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(tasks
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(lines INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_library(Lines::Lines ALIAS lines)
add_library(Lines::Temporal ALIAS temporal)
add_library(Lines::Tasks ALIAS tasks)

enable_testing()

add_executable(tests
    tests/roadmaps/roadmap_tests.cpp
    tests/tasks/task_tests.cpp
    tests/tasks/task_completion_tests.cpp
    tests/tasks/task_info_tests.cpp
    tests/tasks/task_repeat_tests.cpp
    tests/temporal/date_tests.cpp
    tests/temporal/duration_tests.cpp
    tests/temporal/timestamp_tests.cpp
    tests/temporal/timepoint_tests.cpp
    tests/temporal/ymd_tests.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest_main Lines::Temporal Lines::Tasks)

add_test(NAME tests COMMAND tests)
