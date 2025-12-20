#pragma once

#define LATER_VERSION "0.1.0a"
#define LATER_VERSION_MAJOR 0
#define LATER_VERSION_MINOR 1
#define LATER_VERSION_PATCH 0
#define LATER_VERSION_BETA false
#define LATER_VERSION_ALPHA true

#if defined(_WIN32)
#include <windows.h>
#define LATER_WINDOWSNT
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#define LATER_DARWIN
#elif defined(__unix__) || defined(__unix)
#define LATER_UNIX
#else
#error "Unsupported platform"
#endif
