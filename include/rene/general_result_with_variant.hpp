// RENE: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

#include <memory>
#include <tuple>
#include <variant>

#include "rene_internal/rene_optional.hpp"

namespace rene {
namespace internal {

template <typename ErrorT, typename... ValueTs>
class general_result_with_variant_base {
  general_result_with_variant_base(const general_result_with_variant_base&) =
      delete;
  general_result_with_variant_base& operator=(
      const general_result_with_variant_base&) = delete;

  static_assert(sizeof...(ValueTs) >= 1);

  using ErrorPtrT = std::unique_ptr<ErrorT>;
  using ValueTupleT = std::tuple<ValueTs...>;
  using ValueContainerT = std::variant<ErrorPtrT, ValueTupleT>;

  ValueContainerT _data;

 public:
  template <typename Arg1, typename... Args>
  constexpr general_result_with_variant_base(
      Arg1&& arg1,
      Args&&... args) noexcept(std::
                                   is_nothrow_constructible_v<
                                       ValueContainerT,
                                       std::in_place_index_t<1>,
                                       Arg1,
                                       Args...>)
      : _data(std::in_place_index_t<1>{},
              std::forward<Arg1>(arg1),
              std::forward<Args>(args)...) {}

  general_result_with_variant_base(ErrorT&& error)
      : _data(std::in_place_index_t<0>{},
              std::make_unique<ErrorT>(std::move(error))) {}

  general_result_with_variant_base(std::unique_ptr<ErrorT>&& error) noexcept(
      std::is_nothrow_constructible_v<ValueContainerT,
                                      std::in_place_index_t<0>,
                                      std::unique_ptr<ErrorT>&&>&&
          std::is_nothrow_destructible_v<ValueTupleT>)
      : _data(std::in_place_index_t<0>{}, std::move(error)) {}

  general_result_with_variant_base(
      general_result_with_variant_base&&
          other) noexcept(std::is_nothrow_move_constructible_v<ValueContainerT>)
      : _data(std::move(other._data)) {}

  general_result_with_variant_base&
  operator=(general_result_with_variant_base&& other) noexcept(
      std::is_nothrow_move_assignable_v<ValueContainerT>) {
    _data = std::move(other._data);
    return *this;
  }

  template <typename U,
            typename = typename std::enable_if_t<
                std::is_convertible_v<U, ValueTupleT>>>
  general_result_with_variant_base& operator=(U&& tup) noexcept(
      std::is_nothrow_assignable_v<ValueContainerT, U>) {
    _data = std::forward<U>(tup);
    return *this;
  }

  general_result_with_variant_base& operator=(ErrorT&& error) {
    _data = std::make_unique<ErrorT>(std::move(error));
    return *this;
  }

  general_result_with_variant_base&
  operator=(std::unique_ptr<ErrorT>&& error_ptr) noexcept(
      std::is_nothrow_assignable_v<ValueContainerT, std::unique_ptr<ErrorT>&&>&&
          std::is_nothrow_destructible_v<ValueTupleT>) {
    _data = std::move(error_ptr);
    return *this;
  }

  constexpr bool is_ok() const noexcept { return _data.index() != 0; }
  constexpr bool is_err() const noexcept { return _data.index() == 0; }
  constexpr bool is_error() const noexcept { return _data.index() == 0; }

  constexpr std::unique_ptr<ErrorT>&& unchecked_release_error() && noexcept {
    return std::get<0>(std::move(_data));
  }

  constexpr const ErrorT& get_error() const& { return *std::get<0>(_data); }

  constexpr ErrorT& get_error() & { return *std::get<0>(_data); }

  constexpr ErrorT&& get_error() && { *std::get<0>(std::move(_data)); }

  constexpr const std::tuple<ValueTs...>& operator*() const& {
    return std::get<1>(_data);
  }

  constexpr std::tuple<ValueTs...>& operator*() & { return std::get<1>(_data); }

  constexpr std::tuple<ValueTs...>&& operator*() && {
    return std::get<1>(std::move(_data));
  }

  template <size_t I>
  constexpr typename std::tuple_element<I, std::tuple<ValueTs...>>::type& get()
      const& {
    return std::get<I>(std::get<1>(_data));
  }

  template <size_t I>
  typename std::tuple_element<I, std::tuple<ValueTs...>>::type& get() & {
    return std::get<I>(std::get<1>(_data));
  }

  template <size_t I>
  typename std::tuple_element<I, std::tuple<ValueTs...>>::type&& get() && {
    return std::get<I>(std::get<1>(std::move(_data)));
  }
};

}  // namespace internal

// TODO:MP nodiscard
template <typename ErrorT, typename... ResultTs>
class /*RENE_NO_DISCARD*/ general_result_with_variant;

template <typename ErrorT, typename... ValueTs>
class general_result_with_variant
    : private internal::general_result_with_variant_base<ErrorT, ValueTs...> {
 private:
  using BaseT = internal::general_result_with_variant_base<ErrorT, ValueTs...>;

 public:
  template <typename... Args>
  constexpr general_result_with_variant(Args&&... args) noexcept(
      std::is_nothrow_constructible_v<BaseT, Args...>)
      : BaseT(std::forward<Args>(args)...) {}

  template <typename Arg>
  general_result_with_variant& operator=(Arg&& arg) noexcept(
      std::is_nothrow_assignable_v<BaseT, Arg>) {
    BaseT::operator=(std::forward<Arg>(arg));
    return *this;
  }

 public:
  using BaseT::get;
  using BaseT::get_error;
  using BaseT::is_err;
  using BaseT::is_error;
  using BaseT::is_ok;
  using BaseT::unchecked_release_error;
  using BaseT::operator*;
};

template <typename ErrorT, typename ValueT>
class general_result_with_variant<ErrorT, ValueT>
    : private internal::general_result_with_variant_base<ErrorT, ValueT> {
 private:
  using BaseT = internal::general_result_with_variant_base<ErrorT, ValueT>;

 public:
  template <typename... Args>
  constexpr general_result_with_variant(Args&&... args) noexcept(
      std::is_nothrow_constructible_v<BaseT, Args...>)
      : BaseT(std::forward<Args>(args)...) {}

  template <typename Arg>
  general_result_with_variant& operator=(Arg&& arg) noexcept(
      std::is_nothrow_assignable_v<BaseT, Arg>) {
    BaseT::operator=(std::forward<Arg>(arg));
    return *this;
  }

 public:
  using BaseT::get_error;
  using BaseT::is_err;
  using BaseT::is_error;
  using BaseT::is_ok;
  using BaseT::unchecked_release_error;

 public:
  const ValueT& operator*() const& { return BaseT::template get<0>(); }

  ValueT& operator*() & { return BaseT::template get<0>(); }

  ValueT&& operator*() && { return std::move(*this).BaseT::template get<0>(); }

  const std::remove_reference_t<ValueT>* operator->() const {
    return std::addressof(BaseT::template get<0>());
  }

  std::remove_reference_t<ValueT>* operator->() {
    return std::addressof(BaseT::template get<0>());
  }

  const ValueT& get() const& { return BaseT::template get<0>(); }

  ValueT& get() & { return BaseT::template get<0>(); }

  ValueT&& get() && { return std::move(*this).BaseT::template get<0>(); }
};

class no_error_exception final : public std::exception {};

template <typename ErrorT>
class general_result_with_variant<ErrorT> {
  general_result_with_variant(const general_result_with_variant&) = delete;
  general_result_with_variant& operator=(const general_result_with_variant&) =
      delete;

 public:
  using ErrorPtrT = std::unique_ptr<ErrorT>;

 protected:
  ErrorPtrT _error;

 public:
  constexpr general_result_with_variant() noexcept(
      std::is_nothrow_constructible_v<ErrorPtrT>) {}

  general_result_with_variant(ErrorT&& error)
      : _error(std::make_unique<ErrorT>(std::move(error))) {}

  general_result_with_variant(std::unique_ptr<ErrorT>&& error) noexcept(
      std::is_nothrow_move_constructible_v<ErrorPtrT>)
      : _error(std::move(error)) {}

  general_result_with_variant(general_result_with_variant&& other) noexcept(
      std::is_nothrow_move_constructible_v<ErrorPtrT>)
      : _error(std::move(other._error)) {}

  general_result_with_variant&
  operator=(general_result_with_variant&& res) noexcept(
      std::is_nothrow_move_assignable_v<ErrorPtrT>) {
    this->_error = std::move(res._error);
    return *this;
  }

  ~general_result_with_variant() noexcept(
      std::is_nothrow_destructible<ErrorT>::value) {}

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

}  // namespace rene