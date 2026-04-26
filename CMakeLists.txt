cmake_minimum_required(VERSION 3.14)
project(lines-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(LINES_ENABLE_TESTING "Build tests" OFF)

if(MSVC)
  # MSVC historically reports __cplusplus as 199711L,
  # so forcing it to report correct value
  add_compile_options(/Zc:__cplusplus)
endif()

add_library(lines INTERFACE)

add_subdirectory(src)

target_link_libraries(lines INTERFACE
  Lines::Tasks Lines::Temporal Lines::Roadmaps)

target_include_directories(lines INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

add_library(Lines::Lines ALIAS lines)

if (LINES_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
