#!/usr/bin/bash

ctest --preset coverage

lcov \
  --capture \
  --directory build/coverage \
  --output-file coverage.info \
  --ignore-errors mismatch,version,empty,unused \

lcov \
  --remove coverage.info \
  '/usr/*' \
  '*/_deps/*' \
  '*tests*' \
  --output-file coverage.info
