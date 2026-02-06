/*
  #        #  #     #  # # # #  # # # #
  #        #  # #   #  #        #
  #        #  #   # #  # # # #  # # # #
  #        #  #     #  #              #
  # # # #  #  #     #  # # # #  # # # #
  Copyright (c) 2025-2026 I.H.Y.A.D.

  Lines Project, Core library.
  This file is licensed under GNU Lesser General Public License v3.0 or later.
  See LICENSE for more information.
  SPDX-License-Identifier: LGPL-3.0-or-later.
*/
#pragma once

#if defined(_WIN32)
#define NOMINMAX
#define LINES_WINDOWSNT
#elif defined(__APPLE__)
#define LINES_DARWIN
#elif defined(__unix__) || defined(__unix)
#define LINES_UNIX
#endif

#ifdef __cplusplus
#define LINES_CPP __cplusplus
#else
#define LINES_CPP 0
#endif

#ifndef NDEBUG
#define LINES_DEBUG 1
#else
#define LINES_DEBUG 0
#endif

#if LINES_DEBUG
#include <cassert>
#define LINES_ASSERT(cond) assert(cond)
#else
#define LINES_ASSERT(cond) ((void)0)
#endif

#if defined(__has_feature)
#define LINES_HAS_FEATURE(x) __has_feature(x)
#else
#define LINES_HAS_FEATURE(x) 0
#endif

#if defined(__has_builtin)
#define LINES_HAS_BUILTIN(x) __has_builtin(x)
#else
#define LINES_HAS_BUILTIN(x) 0
#endif

#if defined(__has_attribute)
#define LINES_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#define LINES_HAS_ATTRIBUTE(x) 0
#endif

#if defined(__has_cpp_attribute)
#define LINES_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define LINES_HAS_CPP_ATTRIBUTE(x) 0
#endif

#if LINES_HAS_BUILTIN(__builtin_unreachable)
#define LINES_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define LINES_UNREACHABLE() __assume(0)
#else
#define LINES_UNREACHABLE() LINES_ASSERT(0)
#endif

#if LINES_HAS_ATTRIBUTE(visibility)
#define LINES_HAS_VISIBILITY 1
#else
#define LINES_HAS_VISIBILITY 0
#endif

#if LINES_HAS_VISIBILITY
#define LINES_VISIBILITY(x) __attribute__((visibility(x)))
#else
#define LINES_VISIBILITY(x)
#endif
#if defined(LINES_SHARED)
#if defined(_WIN32)
#if defined(LINES_BUILD)
#define LINES_API __declspec(dllexport)
#else
#define LINES_API __declspec(dllimport)
#endif
#else
#if LINES_HAS_VISIBILITY
#define LINES_API LINES_VISIBILITY("default")
#else
#define LINES_API
#endif
#endif
#else
#define LINES_API
#endif

#if LINES_HAS_CPP_ATTRIBUTE(nodiscard)
#define LINES_NODISCARD [[nodiscard]]
#else
#define LINES_NODISCARD
#endif

#if LINES_HAS_CPP_ATTRIBUTE(noreturn)
#define LINES_NORETURN [[noreturn]]
#else
#define LINES_NORETURN
#endif

#if LINES_HAS_CPP_ATTRIBUTE(deprecated)
#define LINES_DEPRECATED(msg) [[deprecated(msg)]]
#elif LINES_HAS_ATTRIBUTE(deprecated)
#define LINES_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define LINES_DEPRECATED(msg)
#endif

#if defined(LINES_SHARED) && !defined(LINES_WINDOWSNT)
#if LINES_HAS_VISIBILITY
#define LINES_HIDE_FROM_ABI LINES_VISIBILITY("hidden")
#else
#define LINES_HIDE_FROM_ABI
#endif
#else
#define LINES_HIDE_FROM_ABI
#endif

#if LINES_CPP >= 201103L
#define LINES_CONSTEXPR constexpr
#else
#define LINES_CONSTEXPR
#endif

#if LINES_CPP >= 201103L
#define LINES_NOEXCEPT noexcept
#else
#define LINES_NOEXCEPT
#endif

#if LINES_CPP >= 201703L
#define LINES_CONSTEXPR_IF if constexpr
#else
#define LINES_CONSTEXPR_IF if
#endif
