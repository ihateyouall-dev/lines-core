include(FetchContent)
FetchContent_Declare(
  googletest
  URL "https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip"
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB LINES_TEMPORAL_TESTS "temporal/*_tests.cpp")

file(GLOB LINES_TASKS_TESTS "tasks/*_tests.cpp")

file(GLOB LINES_ROADMAPS_TESTS "roadmaps/*_tests.cpp")

set(LINES_TESTS
  ${LINES_TEMPORAL_TESTS}
  ${LINES_TASKS_TESTS}
  ${LINES_ROADMAPS_TESTS})

add_executable(tests ${LINES_TESTS})

target_link_libraries(tests PRIVATE GTest::gtest_main Lines::Lines)

add_test(NAME tests COMMAND tests)
