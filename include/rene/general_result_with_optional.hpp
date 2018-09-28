// RENE: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include "rene_internal/rene_optional.hpp"

namespace rene {
namespace internal {

class no_error_exception final : public std::exception {};

template <typename ErrorT>
class result_base {
  result_base(const result_base&) = delete;
  result_base& operator=(const result_base&) = delete;

 public:
  using ErrorPtrT = std::unique_ptr<ErrorT>;

 protected:
  ErrorPtrT _error;

 public:  // <-- because declval can only work like this
  constexpr result_base() noexcept(std::is_nothrow_constructible_v<ErrorPtrT>) {
  }

  result_base(ErrorPtrT&& error) noexcept(
      std::is_nothrow_move_constructible_v<ErrorPtrT>)
      : _error(std::move(error)) {}

  result_base(result_base&& other) noexcept(
      std::is_nothrow_move_constructible_v<ErrorPtrT>) = default;

  constexpr static bool is_nothrow_set_error = false;

  void set_error(ErrorT&& error) {
    if (_error == nullptr) {
      _error = std::make_unique<ErrorT>(std::move(error));
    } else {
      *_error = std::move(error);
    }
  }

  void throw_if_error() const {
    if (_error != nullptr)
      throw *_error;
  }

  ~result_base() noexcept(std::is_nothrow_destructible<ErrorT>::value) {}

 public:
  bool is_ok() const noexcept { return _error == nullptr; }
  bool is_err() const noexcept { return _error != nullptr; }
  bool is_error() const noexcept { return _error != nullptr; }

  std::unique_ptr<ErrorT>&& unchecked_release_error() && noexcept {
    return std::move(_error);
  }

  const ErrorT& get_error() const& {
    if (_error == nullptr)
      throw no_error_exception();
    return *reinterpret_cast<const ErrorT*>(_error.get());
  }

  ErrorT& get_error() & {
    if (_error == nullptr)
      throw no_error_exception();
    return *reinterpret_cast<ErrorT*>(_error.get());
  }

  ErrorT&& get_error() && {
    if (_error == nullptr)
      throw no_error_exception();
    return std::move(*reinterpret_cast<ErrorT*>(_error.get()));
  }
};

}  // namespace internal

// TODO:MP nodiscard
template <typename ErrorT, typename... ResultTs>
class /*RENE_NO_DISCARD*/ general_result_with_optional;

template <typename ErrorT, typename... ResultTs>
class general_result_with_optional : public internal::result_base<ErrorT> {
  general_result_with_optional(const general_result_with_optional&) = delete;
  general_result_with_optional& operator=(const general_result_with_optional&) =
      delete;

  static_assert(sizeof...(ResultTs) >= 2);

  using BaseT = internal::result_base<ErrorT>;
  using ErrorPtrT = typename internal::result_base<ErrorT>::ErrorPtrT;
  using ValueTupleT = std::tuple<ResultTs...>;
  using ValueContainerT = internal::optional<ValueTupleT>;

  ValueContainerT _results;

 public:
  template <typename Arg1, typename... Args>
  constexpr general_result_with_optional(Arg1&& arg1, Args&&... args) noexcept(
      std::is_nothrow_constructible_v<ValueTupleT, Arg1, Args...>&&
          std::is_nothrow_constructible_v<BaseT>)
      : _results(internal::in_place_t{},
                 std::forward<Arg1>(arg1),
                 std::forward<Args>(args)...) {}

  general_result_with_optional(ErrorT&& error)
      : BaseT(std::make_unique<ErrorT>(std::move(error))) {}

  general_result_with_optional(ErrorPtrT&& error) noexcept(
      std::is_nothrow_constructible_v<ValueContainerT>&&
          std::is_nothrow_constructible_v<BaseT, ErrorPtrT&&>)
      : BaseT(std::move(error)) {}

  general_result_with_optional(general_result_with_optional&& other) noexcept(
      std::is_nothrow_move_constructible_v<ValueContainerT>&&
          std::is_nothrow_move_constructible_v<BaseT>)
      : BaseT(std::move(other)), _results(std::move(other._results)) {}

  general_result_with_optional&
  operator=(general_result_with_optional&& other) noexcept(
      std::is_nothrow_move_assignable_v<ValueContainerT>&&
          std::is_nothrow_move_assignable_v<ErrorPtrT>) {
    _results = std::move(other._results);
    this->_error = std::move(other._error);
    return *this;
  }

  template <typename U,
            typename = typename std::enable_if_t<
                std::is_convertible_v<U, ValueTupleT>>>
  general_result_with_optional& operator=(U&& tup) noexcept(
      std::is_nothrow_assignable_v<ValueTupleT, U>&&
          std::is_nothrow_constructible_v<ValueTupleT, U>&&
              std::is_nothrow_destructible_v<ErrorT>) {
    _results = std::forward<U>(tup);
    this->_error.reset();
    return *this;
  }

  general_result_with_optional& operator=(ErrorT&& error) noexcept(
      std::is_nothrow_destructible_v<ValueTupleT>&&
          BaseT::is_nothrow_set_error) {
    _results.reset();
    this->set_error(std::move(error));
    return *this;
  }

  general_result_with_optional& operator=(ErrorPtrT&& error_ptr) noexcept(
      std::is_nothrow_destructible_v<ValueTupleT>&&
          std::is_nothrow_move_assignable_v<ErrorPtrT>) {
    _results.reset();
    this->_error = std::move(error_ptr);
    return *this;
  }

  const std::tuple<ResultTs...>& operator*() const& {
    this->throw_if_error();
    return *_results;
  }

  std::tuple<ResultTs...>& operator*() & {
    this->throw_if_error();
    return *_results;
  }

  std::tuple<ResultTs...>&& operator*() && {
    this->throw_if_error();
    return std::move(*_results);
  }

  template <size_t I>
  const typename std::tuple_element<I, std::tuple<ResultTs...>>::type& get()
      const& {
    this->throw_if_error();
    return std::get<I>(*_results);
  }

  template <size_t I>
  typename std::tuple_element<I, std::tuple<ResultTs...>>::type& get() & {
    this->throw_if_error();
    return std::get<I>(*_results);
  }

  template <size_t I>
  typename std::tuple_element<I, std::tuple<ResultTs...>>::type&& get() && {
    this->throw_if_error();
    return std::get<I>(std::move(*_results));
  }
};

template <typename ErrorT, typename ResultT>
class general_result_with_optional<ErrorT, ResultT>
    : public internal::result_base<ErrorT> {
  general_result_with_optional(const general_result_with_optional&) = delete;
  general_result_with_optional& operator=(const general_result_with_optional&) =
      delete;

  using BaseT = internal::result_base<ErrorT>;
  using ErrorPtrT = typename internal::result_base<ErrorT>::ErrorPtrT;
  using ValueT = ResultT;
  using ValueTupleT = std::tuple<ValueT>;
  using ValueContainerT = internal::optional<ValueTupleT>;

  ValueContainerT _result;

 public:
  template <
      typename U,
      typename = typename std::enable_if_t<std::is_convertible_v<U, ValueT>>>
  constexpr general_result_with_optional(U&& val) noexcept(
      std::is_nothrow_constructible_v<ValueT, U>&&
          std::is_nothrow_constructible_v<BaseT>)
      : _result(internal::in_place_t{}, std::forward<U>(val)) {}

  general_result_with_optional(ErrorT&& error)
      : BaseT(std::make_unique<ErrorT>(std::move(error))) {}

  general_result_with_optional(ErrorPtrT&& error) noexcept(
      std::is_nothrow_constructible_v<ValueContainerT>&&
          std::is_nothrow_constructible_v<BaseT, ErrorPtrT&&>)
      : BaseT(std::move(error)) {}

  general_result_with_optional(general_result_with_optional&& other) noexcept(
      std::is_nothrow_move_constructible_v<ValueT>&&
          std::is_nothrow_move_constructible_v<BaseT>)
      : BaseT(std::move(other)), _result(std::move(other._result)) {}

  general_result_with_optional&
  operator=(general_result_with_optional&& other) noexcept(
      std::is_nothrow_move_assignable_v<ValueContainerT>&&
          std::is_nothrow_move_assignable_v<ErrorPtrT>) {
    _result = std::move(other._result);
    this->_error = std::move(other._error);
    return *this;
  }

  template <
      typename U,
      typename = typename std::enable_if_t<std::is_convertible_v<U, ValueT>>>
  general_result_with_optional& operator=(U&& val) noexcept(
      std::is_nothrow_assignable_v<ValueT, U>&&
          std::is_nothrow_constructible_v<ValueT, U>&&
              std::is_nothrow_destructible_v<ErrorT>) {
    _result = std::forward<U>(val);
    this->_error.reset();
    return *this;
  }

  general_result_with_optional& operator=(ErrorT&& error) noexcept(
      std::is_nothrow_destructible_v<ValueT>&& BaseT::is_nothrow_set_error) {
    _result.reset();
    this->set_error(std::move(error));
    return *this;
  }

  general_result_with_optional&
  operator=(std::unique_ptr<ErrorT>&& error_ptr) noexcept(
      std::is_nothrow_destructible_v<ValueT>&&
          std::is_nothrow_move_assignable_v<ErrorPtrT>) {
    _result.reset();
    this->_error = std::move(error_ptr);
    return *this;
  }

  const ValueT& operator*() const& {
    this->throw_if_error();
    return std::get<0>(*_result);
  }

  ValueT& operator*() & {
    this->throw_if_error();
    return std::get<0>(*_result);
  }

  ValueT&& operator*() && {
    this->throw_if_error();
    return std::move(std::get<0>(*_result));
  }

  const std::remove_reference_t<ValueT>* operator->() const {
    this->throw_if_error();
    return std::addressof(std::get<0>(*_result));
  }

  std::remove_reference_t<ValueT>* operator->() {
    this->throw_if_error();
    return std::addressof(std::get<0>(*_result));
  }

  const ValueT& get() const& {
    this->throw_if_error();
    return std::get<0>(*_result);
  }

  ValueT& get() & {
    this->throw_if_error();
    return std::get<0>(*_result);
  }

  ValueT&& get() && {
    this->throw_if_error();
    return std::get<0>(*_result);
  }
};

template <typename ErrorT>
class general_result_with_optional<ErrorT>
    : public internal::result_base<ErrorT> {
  general_result_with_optional(const general_result_with_optional&) = delete;
  general_result_with_optional& operator=(const general_result_with_optional&) =
      delete;

  using BaseT = internal::result_base<ErrorT>;
  using ErrorPtrT = typename internal::result_base<ErrorT>::ErrorPtrT;

 public:
  constexpr general_result_with_optional() noexcept(
      std::is_nothrow_constructible_v<BaseT>) {}

  general_result_with_optional(ErrorT&& error)
      : BaseT(std::make_unique<ErrorT>(std::move(error))) {}

  general_result_with_optional(ErrorPtrT&& error) noexcept(
      std::is_nothrow_constructible_v<BaseT, ErrorPtrT&&>)
      : BaseT(std::move(error)) {}

  general_result_with_optional(general_result_with_optional&& other) noexcept(
      std::is_nothrow_move_constructible_v<BaseT>) = default;

  general_result_with_optional&
  operator=(general_result_with_optional&& res) noexcept(
      std::is_nothrow_move_assignable_v<ErrorPtrT>) {
    this->_error = std::move(res._error);
    return *this;
  }
};

}  // namespace rene