// RENE: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

#include <exception>
#include <memory>
#include <typeinfo>

#include "rene_internal/rene_common.hpp"

namespace rene {
namespace internal {

template <typename ErrorT, typename... Args>
std::unique_ptr<ErrorT> add_frame(std::unique_ptr<ErrorT> err, Args&&... args) {
  return std::make_unique<ErrorT>(std::move(err), std::forward<Args>(args)...);
}

class error_base : public std::exception {
 public:
  using MessageT = const char*;
  using FileT = const char*;

  template <typename T>
  using CasudeByPtrT = std::unique_ptr<T>;
  using CausedByT = CasudeByPtrT<error_base>;

 private:
  MessageT _message;
  FileT _file;
  uint32_t _line;
  CausedByT _caused_by;

 public:  // <-- because declval can only work like this
  error_base(const MessageT& message, FileT&& file, uint32_t line) noexcept(
      std::is_nothrow_constructible_v<std::exception>&&
          std::is_nothrow_copy_constructible_v<MessageT>&&
              std::is_nothrow_move_constructible_v<FileT>)
      : _message(message), _file(std::move(file)), _line(line) {}

  error_base(MessageT&& message, FileT&& file, uint32_t line) noexcept(
      std::is_nothrow_constructible_v<std::exception>&&
          std::is_nothrow_move_constructible_v<MessageT>&&
              std::is_nothrow_move_constructible_v<FileT>)
      : _message(std::move(message)), _file(std::move(file)), _line(line) {}

  error_base(CausedByT&& caused_by, FileT file, uint32_t line) noexcept(
      std::is_nothrow_move_constructible_v<CausedByT>&&
          std::is_nothrow_move_constructible_v<FileT>)
      : _message(caused_by->_message),
        _file(file),
        _line(line),
        _caused_by(std::move(caused_by)) {}

  error_base(const error_base& other)
      : _message(other._message), _file(other._file), _line(other._line) {
    if (other._caused_by != nullptr)
      _caused_by.reset(new error_base(*other._caused_by));
  }

  error_base(error_base&& other) noexcept(
      std::is_nothrow_move_constructible_v<CausedByT>&&
          std::is_nothrow_move_constructible_v<MessageT>&&
              std::is_nothrow_move_constructible_v<FileT>) = default;

  error_base& operator=(error_base&& other) noexcept(
      std::is_nothrow_move_assignable_v<CausedByT>&&
          std::is_nothrow_move_assignable_v<MessageT>&&
              std::is_nothrow_move_assignable_v<FileT>) = default;

  constexpr static bool is_nothrow_set_caused_by =
      std::is_nothrow_move_assignable_v<CausedByT>;

  void set_caused_by(std::unique_ptr<error_base>&& caused_by) noexcept(
      is_nothrow_set_caused_by) {
    _caused_by = std::move(caused_by);
  }

 public:
  ~error_base() noexcept(std::is_nothrow_destructible_v<CausedByT>) = default;

  virtual char const* what() const noexcept override {
    return _message;  // TODO:MP what if null: caused_by
  }

  const MessageT& msg() const noexcept { return _message; }

  uint32_t line() const noexcept { return _line; }
};
}  // namespace internal

template <typename SpecialErrorT>
class RENE_NODISCARD general_error;

template <typename SpecialErrorT>
class general_error final : public internal::error_base {
  SpecialErrorT _err;

 public:
  constexpr static bool spec_is_nothrow_comperable =
      noexcept(std::declval<SpecialErrorT>() == std::declval<SpecialErrorT>());

 public:
  general_error(
      CasudeByPtrT<general_error>&& caused_by,
      FileT file,
      uint32_t line) noexcept(std::is_nothrow_constructible_v<error_base,
                                                              const MessageT&,
                                                              FileT&&,
                                                              uint32_t>&&
                                  std::is_nothrow_copy_constructible_v<
                                      SpecialErrorT>&& is_nothrow_set_caused_by)
      : error_base(caused_by->msg(), std::move(file), line),
        _err(caused_by->_err) {
    this->set_caused_by(std::move(caused_by));
  }

  general_error(SpecialErrorT&& err, FileT file, uint32_t line) noexcept(
      std::is_nothrow_constructible_v<error_base,
                                      decltype("SpecialErrorT"),
                                      FileT&&,
                                      uint32_t>&&
          std::is_nothrow_move_constructible_v<SpecialErrorT>)
      : error_base("SpecialErrorT", std::move(file), line),
        _err(std::move(err)) {}

  general_error(const general_error& other) noexcept(
      std::is_nothrow_copy_constructible_v<error_base>&&
          std::is_nothrow_copy_constructible_v<SpecialErrorT>) = default;

  general_error(general_error&& other) noexcept(
      std::is_nothrow_move_constructible_v<error_base>&&
          std::is_nothrow_move_constructible_v<SpecialErrorT>)
      : error_base(std::move(other)), _err(std::move(other._err)) {}

  general_error& operator=(general_error&& other) noexcept(
      std::is_nothrow_move_assignable_v<error_base>&&
          std::is_nothrow_move_assignable_v<SpecialErrorT>) {
    this->error_base::operator=(std::move(other));
    _err = std::move(other._err);
    return *this;
  }

  virtual ~general_error() noexcept(
      std::is_nothrow_destructible<error_base>::value&&
          std::is_nothrow_destructible<SpecialErrorT>::value) {}

  // virtual const char* what()const noexcept override {
  // typeid(SpecialErrorT).name() /*TODO:MP copy*/}

  constexpr bool is(SpecialErrorT err) const
      noexcept(spec_is_nothrow_comperable) {
    return err == _err;
  }

  constexpr SpecialErrorT& get() const& noexcept { return _err; }

  SpecialErrorT& get() & noexcept { return _err; }
  SpecialErrorT&& get() && noexcept { return std::move(_err); }

  void set(const SpecialErrorT& errCode) noexcept(
      std::is_nothrow_copy_assignable<SpecialErrorT>::value) {
    _err = errCode;
  }

  void set(SpecialErrorT&& errCode) noexcept(
      std::is_nothrow_move_assignable<SpecialErrorT>::value) {
    _err = std::move(errCode);
  }
};

template <>
class general_error<void> final : public internal::error_base {
 public:
  general_error(CasudeByPtrT<general_error>&& caused_by,
                FileT&& file,
                uint32_t line) noexcept(std::
                                            is_nothrow_constructible_v<
                                                error_base,
                                                CasudeByPtrT<general_error>&&,
                                                FileT&&,
                                                uint32_t>)
      : error_base(std::move(caused_by), std::move(file), line) {}

  general_error(MessageT&& message, FileT&& file, uint32_t line) noexcept(
      std::
          is_nothrow_constructible_v<error_base, MessageT&&, FileT&&, uint32_t>)
      : error_base(std::move(message), std::move(file), line) {}

  general_error(const general_error& other) noexcept(
      std::is_nothrow_copy_constructible_v<error_base>) = default;

  general_error(general_error&& other) noexcept(
      std::is_nothrow_move_constructible_v<error_base>) = default;

  general_error& operator=(general_error&& other) noexcept(
      std::is_nothrow_move_assignable_v<error_base>) = default;

  virtual ~general_error() noexcept(
      std::is_nothrow_destructible_v<error_base>) = default;
};

}  // namespace rene
