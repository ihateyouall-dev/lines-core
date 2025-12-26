cmake_minimum_required(VERSION 3.14)
project(later LANGUAGES CXX)

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

add_library(tasks INTERFACE)
add_library(macro INTERFACE)
add_library(temporal INTERFACE)

target_include_directories(tasks INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/later/tasks>)
target_include_directories(macro INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/later/macro>)
target_include_directories(temporal INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/later/temporal>)

target_link_libraries(tasks INTERFACE macro temporal)

add_library(Later::Tasks ALIAS tasks)
add_library(Later::Temporal ALIAS temporal)

enable_testing()

add_executable(tests
    tests/temporal/duration_tests.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest_main Later::Temporal)

add_test(NAME tests COMMAND tests)
