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
#include <cstdint>
#include <cassert>

#ifdef FIRE_PLATFORM_WINDOWS
  #ifdef FIRE_SHARED_LIBRARY
    #define FIRE_API __declspec(dllexport)
  #else
    #define FIRE_API __declspec(dllimport)
  #endif
#endif

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

#if defined(__cplusplus) && __cplusplus >= 201103L
#define FIRE_OVERRIDE override
#else
#define FIRE_OVERRIDE
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
#define FIRE_NORETURN [[noreturn]]
#else
#define FIRE_NORETURN
#endif

#if defined(__cplusplus) && __cplusplus >= 201703L
#define FIRE_NODISCARD [[nodiscard]]
#else
#define FIRE_NODISCARD
#endif

#if defined(__cplusplus) && __cplusplus >= 202002L
#include <type_traits>
template <typename T>
concept TrivialType = std::is_trivial_v<T>;
#else
#endif

#define __STR(exp) (#exp)
#define STR(exp) __STR(exp)

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

#define BIT(x) ((1<<x))

template<typename TO, typename FROM>
FIRE_CONSTEXPR bool IsA(FROM* ptr) FIRE_NOEXCEPT {
  return TO::ClassOf(ptr);
}
template<typename TO, typename FROM>
FIRE_CONSTEXPR bool IsA(const FROM* ptr) FIRE_NOEXCEPT {
  return TO::ClassOf(ptr);
}

template<typename TO, typename FROM>
FIRE_CONSTEXPR TO* Cast(FROM* ptr) FIRE_NOEXCEPT {
  FIRE_ASSERT(IsA<TO>(ptr));
  return static_cast<TO*>(ptr);
}
template<typename TO, typename FROM>
FIRE_CONSTEXPR const TO* Cast(const FROM* ptr) FIRE_NOEXCEPT {
  FIRE_ASSERT(IsA<TO>(ptr));
  return static_cast<const TO*>(ptr);
}

template<typename TO, typename FROM>
FIRE_CONSTEXPR TO* DynCast(FROM* ptr) FIRE_NOEXCEPT {
  return IsA<TO>(ptr) ? Cast<TO>(ptr) : nullptr;
}
template<typename TO, typename FROM>
FIRE_CONSTEXPR const TO* DynCast(const FROM* ptr) FIRE_NOEXCEPT {
  return IsA<TO>(ptr) ? Cast<TO>(ptr) : nullptr;
}

using FireResult = uint32_t;
constexpr FireResult FIRE_SUCCESS = 0;
constexpr FireResult FIRE_FAILURE = 1;

#include <vector>
template<typename T>
using Vector = std::vector<T>;

#include <string>
using String = std::string;

#include <tuple>
template<typename T, typename U>
using Pair = std::pair<T, U>;

#include <optional>
template<typename T>
using Optional = std::optional<T>;

#include <memory>
template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename... Args>
FIRE_CONSTEXPR Ref<T> CreateRef(Args&&... args) {
  return std::make_shared<T>(std::forward<args>...);
}

template<typename T>
using Uni = std::unique_ptr<T>;
template<typename T, typename... Args>
FIRE_CONSTEXPR Uni<T> CreateUni(Args&&... args) {
  return std::make_unique<T>(std::forward<args>...);
}

#endif // FIRE_CORE_H
