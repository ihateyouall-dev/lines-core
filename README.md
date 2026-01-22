![LOGO](docs/lines.svg)

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/07b1c322cc364e8d88a33d44f674d584)](https://app.codacy.com?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Codecov](https://img.shields.io/codecov/c/gh/ihateyouall-dev/lines-core?logo=codecov)](https://app.codecov.io/gh/ihateyouall-dev/lines-core)
[![License](https://img.shields.io/github/license/ihateyouall-dev/lines-core)](LICENSE)
[![Build](https://github.com/ihateyouall-dev/lines-core/actions/workflows/build.yml/badge.svg)](https://github.com/ihateyouall-dev/lines-core/actions/workflows/build.yml)
# Lines Core: library for building powerful life planning apps

> Lines Core is the heart of Lines project, it provides core primitives for building life planning and task management applications

### Project status: alpha

## Table of contents
- [Requirements](#requirements)
- [Installation](#installation)
- [Features](#features)
- [Examples](#examples)

## Requirements
- C++20 or newer
- CMake 3.14+

## Installation
Installation can be done via CMake FetchContent:

```cmake
include(FetchContent)

FetchContent_Declare(
    lines
    GIT_REPOSITORY https://github.com/ihateyouall-dev/lines-core
    GIT_TAG main
)

FetchContent_MakeAvailable(lines)
```

For information about CMake API see documentation (WIP)

## Features

- Task Management
- Roadmaps
- Simple and cross-platform time management 
- Note management (WIP)

## Examples

You can easily create a simple TODO-list
```cpp
#include <lines/tasks/task.hpp>
#include <vector>

int main() {
    std::vector<Lines::Task> task_list;
    task_list.emplace_back(Lines::Task{Lines::TaskInfo{"Write README"}});
    task_list.emplace_back(Lines::Task{Lines::TaskInfo{"Fix CI"}});
    task_list.emplace_back(Lines::Task{Lines::TaskInfo{"Do something useless"}});
    
    task_list[0].complete();
    task_list[1].complete();
    task_list[2].skip();
    return 0;
}
```

## Contribution (WIP)
