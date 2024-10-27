/**
  ******************************************************************************
  * @file           : Core.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */


#ifndef FIRE_CORE_H
#define FIRE_CORE_H

#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cassert>

/* Fire Platform */
#ifdef FIRE_PLATFORM_WINDOWS
  #ifdef FIRE_SHARED_LIBRARY
    #define FIRE_API __declspec(dllexport)
  #else
    #define FIRE_API __declspec(dllimport)
  #endif
#endif

/* CXX Features */
#if defined(__cplusplus) && __cplusplus >= 201103L
#define FIRE_NOEXCEPT noexcept
#else
#define FIRE_NOEXCEPT
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
#define FIRE_CONSTEXPR constexpr
#else
#define FIRE_CONSTEXPR
#endif

#if defined(__cplusplus) && __cplusplus >= 201703L
#define FIRE_NODISCARD [[nodiscard]]
#else
#define FIRE_NODISCARD
#endif

#if defined(__cplusplus) && __cplusplus >= 202002L
template <typename T>
concept TrivialType = std::is_trivial_v<T>;
#else
#endif

/* convert a macro to string */
#define __STR(exp) (#exp)
#define STR(exp) __STR(exp)

/* assert for debug */
#ifdef NDEBUG
#define FIRE_ASSERT(exp) do { (void)exp; } while (0)
#else
#define FIRE_ASSERT(exp) do { assert(exp); } while (0)
#endif

#ifdef NDEBUG
#define VK_ASSERT(exp) do { (void)exp; } while (0)
#else
#define VK_ASSERT(exp) do { assert(exp == VK_NULL_HANDLE); } while (0)
#endif

#ifdef NDEBUG
#define SDL_ASSERT(exp) do { (void)exp; } while (0)
#else
#define SDL_ASSERT(exp) do { assert(exp == SDL_TRUE); } while (0)
#endif

#ifdef NDEBUG
#define VK_CHECK(stmt) do { (void)stmt; } while (0)
#else
#define VK_CHECK(stmt) do { assert(stmt == VK_SUCCESS); } while (0)
#endif

using FireResult = uint32_t;
constexpr FireResult FIRE_SUCCESS = 0;
constexpr FireResult FIRE_FAILURE = 1;

#endif // FIRE_CORE_H
