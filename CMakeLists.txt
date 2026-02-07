cmake_minimum_required(VERSION 3.14)
project(lines-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LINES_ENABLE_TESTING ON)

if(MSVC)
  # MSVC historically reports __cplusplus as 199711L, so forcing it to report correct value
  add_compile_options(/Zc:__cplusplus)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(lines INTERFACE)

add_subdirectory(src)

target_link_libraries(lines INTERFACE Lines::Tasks Lines::Temporal Lines::Roadmaps)

target_include_directories(lines INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

add_library(Lines::Lines ALIAS lines)

if (${LINES_ENABLE_TESTING})
  enable_testing()
  add_subdirectory(tests)
endif()
