add_library(tasks)
add_library(temporal)

file(GLOB LINES_TEMPORAL_SOURCES "temporal/*.cpp")
file(GLOB LINES_TASKS_SOURCES "tasks/*.cpp")

target_sources(temporal PRIVATE ${LINES_TEMPORAL_SOURCES})
target_sources(tasks PRIVATE ${LINES_TASKS_SOURCES})

target_include_directories(temporal PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(tasks PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(tasks PUBLIC temporal)

add_library(Lines::Temporal ALIAS temporal)
add_library(Lines::Tasks ALIAS tasks)
