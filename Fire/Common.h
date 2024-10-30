/**
  ******************************************************************************
  * @file           : Common.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



// ReSharper disable CppUnusedIncludeDirective
#ifndef FIRE_COMMON_H
#define FIRE_COMMON_H

/* Handle HRI */
#ifdef FIRE_USE_VULKAN_HRI
  #include <vulkan/vulkan.hpp>
#endif

/* Handle Window */
#ifdef FIRE_USE_SDL3_WINDOW
  #include <SDL3/SDL.h>
  #ifdef FIRE_USE_VULKAN_HRI
    #include <SDL3/SDL_vulkan.h>
  #endif
#endif

/* Frequently Used Headers */
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/* Frequently Used Third Party Headers */
#include <glm/glm.hpp>
#include <Eigen/Core>

namespace FIRE {

/* Handle Platform Different */
#ifdef FIRE_PLATFORM_WINDOWS
  #ifdef FIRE_SHARED_LIBRARY
    #define FIRE_API __declspec(dllexport)
  #else
    #define FIRE_API __declspec(dllimport)
  #endif
#elif defined(FIRE_PLATFORM_LINUX)
  #ifdef FIRE_SHARED_LIBRARY
    #define FIRE_API /* TODO */
  #else
    #define FIRE_API /* TODO */
  #endif
#endif

/* Handle Compiler Difference */
// TODO

/* Handle C/C++ Version Difference */
// C++11
#if defined(__cplusplus) && __cplusplus >= 201103L
#define FIRE_NOEXCEPT noexcept
#define FIRE_CONSTEXPR constexpr
#define FIRE_OVERRIDE override
#define FIRE_NORETURN [[noreturn]]
#else
#define FIRE_NOEXCEPT
#define FIRE_CONSTEXPR
#define FIRE_OVERRIDE
#define FIRE_NORETURN
#endif
// C++17
#if defined(__cplusplus) && __cplusplus >= 201703L
#define FIRE_NODISCARD [[nodiscard]]
#else
#define FIRE_NODISCARD
#endif
// C++20
#if defined(__cplusplus) && __cplusplus >= 202002L
#else
#endif

/* Useful Macros */
#define __FIRE_STR__(exp) (#exp)
#define FIRE_STR(exp) __FIRE_STR__(exp)

#define FIRE_BIT(x) ((1<<x))

#define FIRE_UNUSE(x) ((void)x)
  
#ifdef FIRE_PLATFORM_WINDOWS
  #define FIRE_DEBUGBREAK() do { __debugbreak(); } while(0)
#else
  #define FIRE_DEBUGBREAK do { raise(SIGTRAP); } while(0)
#endif

#ifdef NDEBUG
  #define FIRE_ASSERT(exp)
#else
  #define FIRE_ASSERT(exp) do { assert(exp); FIRE_DEBUGBREAK(); } while (0)
#endif

#define FIRE_EXIT_SUCCESS() do { exit(EXIT_SUCCESS); } while (0)
#define FIRE_EXIT_FAILURE() do { exit(EXIT_FAILURE); FIRE_DEBUGBREAK(); } while (0)
  
/* Unify Naming Style */
using FireResult = uint32_t;
constexpr FireResult FIRE_SUCCESS = 0;
constexpr FireResult FIRE_FAILURE = 1;
                                                using Path = std::filesystem::path;
                                                using String = std::string;
template<typename T>                            using Queue = std::queue<T>;
template<typename T>                            using Atomic = std::atomic<T>;
template<typename T>                            using Vector = std::vector<T>;
template<typename T>                            using Ref = std::shared_ptr<T>;
template<typename T>                            using Uni = std::unique_ptr<T>;
template<typename T>                            using Optional = std::optional<T>;
template<typename T>                            using Function = std::function<T>;
template<typename T>                            using HashSet = std::unordered_set<T>;
template<typename T, typename U>                using Pair = std::pair<T, U>;
template<typename K, typename V>                using HashMap = std::unordered_map<K, V>;
template<typename T, typename C = std::less<T>> using Heap = std::priority_queue<T, Vector<T>, C>;
template<typename T, typename... Args>
FIRE_CONSTEXPR Ref<T> CreateRef(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
template<typename T, typename... Args>
FIRE_CONSTEXPR Uni<T> CreateUni(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

////////////////////////////////////////////LLVM-Style RTTI/////////////////////////////////////////////////////////////

/* Do Not Forget To Add Constructor/Destructor Yourself */

#define FIRE_RTTI_BASE(cls, ...)                                               \
public:                                                                        \
  enum cls##Kind : uint32_t{__VA_ARGS__};                                      \
                                                                               \
private:                                                                       \
  const cls##Kind kind;                                                        \
                                                                               \
public:                                                                        \
  FIRE_CONSTEXPR cls##Kind GetKind() const FIRE_NOEXCEPT { return kind; }      \
                                                                               \
protected:                                                                     \
  explicit cls(const cls##Kind kind) FIRE_NOEXCEPT : kind(kind) {}             \
                                                                               \
public:                                                                        \
  virtual ~cls() FIRE_NOEXCEPT = default;

#define FIRE_RTTI_DERIVE(cls, kind)                                            \
public:                                                                        \
  FIRE_CONSTEXPR static bool ClassOf(const cls *ptr) FIRE_NOEXCEPT {           \
    return ptr->GetKind() == kind;                                             \
  }

template <typename TO, typename FROM>
FIRE_CONSTEXPR bool IsA(FROM *ptr) FIRE_NOEXCEPT {
  if (ptr == nullptr) return false;
  return TO::ClassOf(ptr);
}
  
template <typename TO, typename FROM>
FIRE_CONSTEXPR bool IsA(const FROM *ptr) FIRE_NOEXCEPT {
  if (ptr == nullptr) return false;
  return TO::ClassOf(ptr);
}
  
template <typename TO, typename FROM>
FIRE_CONSTEXPR TO *Cast(FROM *ptr) FIRE_NOEXCEPT {
  FIRE_ASSERT(IsA<TO>(ptr));
  return static_cast<TO *>(ptr);
}
  
template <typename TO, typename FROM>
FIRE_CONSTEXPR const TO *Cast(const FROM *ptr) FIRE_NOEXCEPT {
  FIRE_ASSERT(IsA<TO>(ptr));
  return static_cast<const TO *>(ptr);
}
  
template <typename TO, typename FROM>
FIRE_CONSTEXPR TO *DynCast(FROM *ptr) FIRE_NOEXCEPT {
  return IsA<TO>(ptr) ? Cast<TO>(ptr) : nullptr;
}
  
template <typename TO, typename FROM>
FIRE_CONSTEXPR const TO *DynCast(const FROM *ptr) FIRE_NOEXCEPT {
  return IsA<TO>(ptr) ? Cast<TO>(ptr) : nullptr;
}
  
}

#include "Core/Logger.h"

#endif // FIRE_COMMON_H
