enable_testing()

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
