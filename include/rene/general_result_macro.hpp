#pragma once

#include <memory>

#include "rene/error.hpp"

/**
 *
 */
#define RENE_RETURN(...) _RENE_INNER_RETURN(__COUNTER__, __VA_ARGS__)

/**
 *
 */
#define RENE_RETURN_IF_ERROR(...) _RENE_INNER_RETURN_IF_ERROR(__VA_ARGS__)

/**
 *
 */
#define RENE_RETURN_OR_CREATE(var, ...) \
  _RENE_INNER_RETURN_OR_CREATE(var, __COUNTER__, __VA_ARGS__)

/**
 *
 */
#define RENE_RETURN_OR_ASSIGN(var, ...) \
  _RENE_INNER_RETURN_OR_ASSIGN(var, __COUNTER__, __VA_ARGS__)

/**
 *
 */
#define RENE_HANDLE_IF_ERROR(var, ...) \
  _RENE_INNER_HANDLE_IF_ERROR(var, __COUNTER__, __VA_ARGS__)

/**
 *
 */
#define RENE_RETURN_ERROR(message) \
  return std::make_unique<::rene::error>(message, __FILE__, __LINE__);

/**
 *
 */
#define RENE_RETURN_SPEC_ERROR(value)                                         \
  return std::make_unique<                                                    \
      ::rene::spec_error<std::decay<decltype(value)>::type>>(value, __FILE__, \
                                                             __LINE__);

/**
 *
 */
#define RENE_CATCH_AND_RETURN_ERROR() \
  catch (const std::exception& e) return ERROR(e)

///
///

#define _RENE_INTERNAL_ADD_FRAME(err) \
  ::rene::internal::add_frame((err), __FILE__, __LINE__)

#define _RENE_INNER2_RETURN(counter, ...)                    \
  auto _res##counter = (__VA_ARGS__);                        \
  if (_RENE_INTERNAL_UNLIKELY(_res##counter.is_error())) {   \
    return _RENE_INTERNAL_ADD_FRAME(                         \
        std::move(_res##counter).unchecked_release_error()); \
  }                                                          \
  return _res##counter

#define _RENE_INNER_RETURN(counter, ...) \
  _RENE_INNER2_RETURN(counter, __VA_ARGS__)

#define _RENE_INNER_RETURN_IF_ERROR(...)            \
  do {                                              \
    auto _x = (__VA_ARGS__);                        \
    if (_RENE_INTERNAL_UNLIKELY(_x.is_error())) {   \
      return _RENE_INTERNAL_ADD_FRAME(              \
          std::move(_x).unchecked_release_error()); \
    }                                               \
  } while (false)

#define _RENE_INNER2_RETURN_OR_CREATE(var, counter, ...)     \
  auto _res##counter = (__VA_ARGS__);                        \
  if (_RENE_INTERNAL_UNLIKELY(_res##counter.is_error()))     \
    return _RENE_INTERNAL_ADD_FRAME(                         \
        std::move(_res##counter).unchecked_release_error()); \
  auto&& var = *_res##counter

#define _RENE_INNER_RETURN_OR_CREATE(var, counter, ...) \
  _RENE_INNER2_RETURN_OR_CREATE(var, counter, __VA_ARGS__)

#define _RENE_INNER2_RETURN_OR_ASSIGN(var, counter, ...)     \
  _RENE_INNER2_RETURN_OR_CREATE(_res##counter, __VA_ARGS__); \
  var = std::move(*_res##counter)

#define _RENE_INNER_RETURN_OR_ASSIGN(var, counter, ...) \
  _RENE_INNER2_RETURN_OR_ASSIGN(var, counter, __VA_ARGS__)

#define _RENE_INNER2_HANDLE_IF_ERROR(var, counter, ...)       \
  if (auto _res##counter = (__VA_ARGS__);                     \
      _RENE_INTERNAL_LIKELY(!_res##counter.is_error())) {     \
  } else if (auto&& var = _res##counter.get_error(); false) { \
  } else

#define _RENE_INNER_HANDLE_IF_ERROR(var, counter, ...) \
  _RENE_INNER2_HANDLE_IF_ERROR(var, counter, __VA_ARGS__)

///

// TODO
//#define MAP_AND_RETUN_IF_ERROR(newError, ...)
// do {
//    auto _x = (__VA_ARGS__);
//    if (_x.is_error()) {
//        return newError.addCausedBy(_x.get_error());
//    }
//} while (false)