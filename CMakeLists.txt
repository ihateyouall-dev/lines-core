add_library(tasks INTERFACE)
add_library(macro INTERFACE)
add_library(temporal INTERFACE)

target_include_directories(tasks INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/later/tasks>)
target_include_directories(macro INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/later/macro>)
target_include_directories(temporal INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/later/temporal>)

target_link_libraries(tasks INTERFACE macro temporal)

add_library(Later::Tasks ALIAS tasks)
add_library(Later::Temporal ALIAS temporal)
