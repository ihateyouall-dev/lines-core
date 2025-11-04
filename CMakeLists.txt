cmake_minimum_required(VERSION 3.28)
project(liblater LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(tasks INTERFACE)

target_include_directories(tasks INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/tasks)

target_link_libraries(tasks INTERFACE nlohmann_json::nlohmann_json)

add_library(Later::tasks ALIAS tasks)
