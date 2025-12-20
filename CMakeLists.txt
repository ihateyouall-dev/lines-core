cmake_minimum_required(VERSION 3.28)
project(liblater LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(tasks INTERFACE)
add_library(macro INTERFACE)
add_library(time INTERFACE)

target_include_directories(tasks INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/tasks)
target_include_directories(macro INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/macro)
target_include_directories(time INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/time)

target_link_libraries(tasks INTERFACE macro time)

add_library(Later::Tasks ALIAS tasks)
add_library(Later::Time ALIAS time)
