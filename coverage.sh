#!/usr/bin/bash

ctest --preset coverage

lcov \
  --capture \
  --directory build/coverage \
  --output-file coverage.info \
  --ignore-errors mismatch,version,empty \

lcov \
  --remove coverage.info \
  '/usr/*' \
  '*/_deps/*' \
  '*gtest*' \
  '*tests*' \
  '*clocks*' \
  --output-file coverage.info
