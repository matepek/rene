// JASL: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <tuple>

#include "rene/error.hpp"
#include "rene/rene_internal/rene_optional.hpp"

namespace rene {
namespace internal {

class result_base_no_type {
  result_base_no_type(const result_base_no_type&) = delete;
  result_base_no_type& operator=(const result_base_no_type&) = delete;

  friend class error_base;

 protected:
  std::unique_ptr<internal::error_base> _error;

  constexpr result_base_no_type() noexcept {}

  result_base_no_type(result_base_no_type&& other) noexcept;

  explicit result_base_no_type(
      std::unique_ptr<internal::error_base> error) noexcept;

  ~result_base_no_type() noexcept {}

 public:
  bool is_error() const noexcept { return _error != nullptr; }
};

template <typename ErrorType>
class result_base : public result_base_no_type {
  result_base(const result_base&) = delete;
  result_base& operator=(const result_base&) = delete;

 protected:
  constexpr result_base() noexcept {}

  result_base(result_base&& other) noexcept
      : result_base_no_type(std::move(other)) {}

  result_base(std::unique_ptr<internal::error_base> error) noexcept
      : result_base_no_type(std::move(error)) {}

  ~result_base() noexcept {}

 public:
  const ErrorType& get_error() const& {
    if (_error == nullptr)
      throw std::exception();
    return *reinterpret_cast<const ErrorType*>(_error.get());
  }

  ErrorType& get_error() & {
    if (_error == nullptr)
      throw std::exception();
    return *reinterpret_cast<ErrorType*>(_error.get());
  }

  ErrorType&& get_error() && {
    if (_error == nullptr)
      throw std::exception();
    return std::move(*reinterpret_cast<ErrorType*>(_error.get()));
  }
};

}  // namespace internal

// TODO:MP nodiscard
template <typename ErrorType, typename... ResultTypes>
class general_result;

template <typename ErrorType, typename... ResultTypes>
class general_result : public internal::result_base<ErrorType> {
  general_result(const general_result&) = delete;
  general_result& operator=(const general_result&) = delete;

  static_assert(sizeof...(ResultTypes) >= 2, "sizeof...(ResultTypes) >= 2");

  rene::internal::optional<std::tuple<ResultTypes...>> _results;

 public:
  template <typename Arg1, typename... Args>
  constexpr general_result(Arg1&& arg1, Args&&... args) /*TODO: noexcept*/
      : _results(internal::in_place_t{},
                 std::forward<Arg1>(arg1),
                 std::forward<Args>(args)...) {}

  general_result(general_result&& other) noexcept
      : internal::result_base<ErrorType>(std::move(other)),
        _results(std::move(other._results)) {}

  general_result(ErrorType&& error)
      : internal::result_base<ErrorType>(
            std::make_unique<ErrorType>(std::move(error))) {}

  general_result& operator=(std::tuple<ResultTypes...>&& tup) /*TODO noexcept*/
  {
    _results.emplace(std::move(tup));
    this->_error.reset();
    return *this;
  }

  general_result& operator=(ErrorType&& error) {
    _results.reset();
    this->_error = std::make_unique<ErrorType>(std::move(error));
    return *this;
  }

  const std::tuple<ResultTypes...>& operator*() const& { return *_results; }

  std::tuple<ResultTypes...>& operator*() & { return *_results; }

  std::tuple<ResultTypes...>&& operator*() && { return std::move(*_results); }

  template <size_t I>
  const typename std::tuple_element<I, std::tuple<ResultTypes...>>::type& get()
      const& {
    if (this->is_error())
      throw this->get_error();
    return std::get<I>(*_results);
  }

  template <size_t I>
  typename std::tuple_element<I, std::tuple<ResultTypes...>>::type& get() & {
    if (this->is_error())
      throw this->get_error();
    return std::get<I>(*_results);
  }

  template <size_t I>
  typename std::tuple_element<I, std::tuple<ResultTypes...>>::type&& get() && {
    if (this->is_error())
      throw this->get_error();
    return std::get<I>(std::move(*_results));
  }
};

template <typename ErrorType, typename ResultType>
class general_result<ErrorType, ResultType>
    : public internal::result_base<ErrorType> {
  general_result(const general_result&) = delete;
  general_result& operator=(const general_result&) = delete;

  rene::internal::optional<ResultType> _result;

 public:
  constexpr general_result(ResultType&& val) /*TODO: noexcept*/
      : _result(internal::in_place_t{}, std::move(val)) {}

  general_result(general_result&& other) noexcept
      : internal::result_base<ErrorType>(std::move(other)),
        _result(std::move(other._result)) {}

  general_result(ErrorType&& error)
      : internal::result_base<ErrorType>(
            std::make_unique<ErrorType>(std::move(error))) {}

  general_result& operator=(ResultType&& val) /*TODO: noexcept*/
  {
    _result.emplace(std::move(val));
    this->_error.reset();
    return *this;
  }

  general_result& operator=(ErrorType&& error) {
    _result.reset();
    this->_error = std::make_unique<ErrorType>(std::move(error));
    return *this;
  }

  const ResultType& operator*() const& { return *_result; }

  ResultType& operator*() & { return *_result; }

  ResultType&& operator*() && { return std::move(*_result); }

  const ResultType& get() const& {
    if (this->is_error())
      throw this->get_error();
    return *_result;
  }

  ResultType& get() & {
    if (this->is_error())
      throw this->get_error();
    return *_result;
  }

  ResultType&& get() && {
    if (this->is_error())
      throw this->get_error();
    return *_result;
  }
};

template <typename ErrorType>
class general_result<ErrorType> : public internal::result_base<ErrorType> {
  general_result(const general_result&) = delete;
  general_result& operator=(const general_result&) = delete;

 public:
  constexpr general_result() noexcept {}

  general_result(general_result&& other) noexcept = default;

  general_result(ErrorType&& error)
      : internal::result_base<ErrorType>(
            std::make_unique<ErrorType>(std::move(error))) {}

  general_result& operator=(general_result&& res) {
    this->_error = std::move(res._error);
    return *this;
  }

  general_result& operator=(ErrorType&& error) {
    this->_error = std::make_unique<ErrorType>(std::move(error));
    return *this;
  }
};

template <typename... ResultTypes>
using result = general_result<error, ResultTypes...>;

template <typename ErrorCodeEnum, typename... ResultTypes>
using spec_result = general_result<spec_error<ErrorCodeEnum>, ResultTypes...>;

namespace internal {
template <typename ErrorT, typename... Args>
ErrorT add_frame(general_result<ErrorT, Args...>&& res,
                 const char* file,
                 uint32_t line) {
  assert(res.is_error());
  return ErrorT(std::move(res), file, line);
}
}  // namespace internal

#define _RENE_INTERNAL_ADD_FRAME(err) \
  ::rene::internal::add_frame((err), __FILE__, __LINE__)

#define RETURN(...)                                 \
  auto _x = (__VA_ARGS__);                          \
  if (_x.is_error()) {                              \
    return _RENE_INTERNAL_ADD_FRAME(std::move(_x)); \
  }                                                 \
  return _x;

#define RETURN_IF_ERROR(...)                          \
  do {                                                \
    auto _x = (__VA_ARGS__);                          \
    if (_x.is_error()) {                              \
      return _RENE_INTERNAL_ADD_FRAME(std::move(_x)); \
    }                                                 \
  } while (false)

#define _INNER_RETURN_OR_CREATE(var, counter, ...)             \
  auto _res##counter = (__VA_ARGS__);                          \
  if (_res##counter.is_error())                                \
    return _RENE_INTERNAL_ADD_FRAME(std::move(_res##counter)); \
  auto&& var = *_res##counter

#define RETURN_OR_CREATE(var, ...) \
  _INNER_RETURN_OR_CREATE(var, __COUNTER__, __VA_ARGS__)

#define _INNER_RETURN_OR_ASSIGN(var, counter, ...)     \
  _INNER_RETURN_OR_CREATE(_res##counter, __VA_ARGS__); \
  var = std::move(*_res##counter)

#define RETURN_OR_ASSIGN(var, ...) \
  _INNER_RETURN_OR_ASSIGN(var, __COUNTER__, __VA_ARGS__)

#define HANDLE_IF_ERROR(var, ...)                  \
  if (auto _x = (__VA_ARGS__); !_x.is_error()) {   \
  } else if (auto&& var = _x.get_error(); false) { \
  } else

// TODO
//#define MAP_AND_RETUN_IF_ERROR(newError, ...)
// do {
//    auto _x = (__VA_ARGS__);
//    if (_x.is_error()) {
//        return newError.addCausedBy(_x.get_error());
//    }
//} while (false)

}  // namespace rene
