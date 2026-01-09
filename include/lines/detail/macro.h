#pragma once

#define LINES_VERSION "0.1.0"
#define LINES_VERSION_MAJOR 0
#define LINES_VERSION_MINOR 1
#define LINES_VERSION_PATCH 0
#define LINES_VERSION_BETA false
#define LINES_VERSION_ALPHA false

#if defined(_WIN32)
#define LINES_WINDOWSNT
#elif defined(__APPLE__)
#define LINES_DARWIN
#elif defined(__unix__) || defined(__unix)
#define LINES_UNIX
#else
#error "Unsupported platform"
#endif
