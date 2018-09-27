// JASL: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#include "rene/error.hpp"
#include "rene/result.hpp"

namespace rene {
namespace internal {

error_base::error_base(result_base_no_type&& result_with_caused_by,
                       const char* file,
                       uint32_t line) noexcept
    : _message(nullptr),
      _file(file),
      _line(line),
      _caused_by(std::move(result_with_caused_by._error)) {}

error_base::error_base(const char* message,
                       const char* file,
                       uint32_t line) noexcept
    : _message(message), _file(file), _line(line) {}

error_base::error_base(error_base&& other, const char* file, uint32_t line)
    : _message(), _file(file), _line(line) {
  if (file != _file || line != _line) {
    _caused_by.reset(new error_base(std::move(other)));
    _message = other._message;
  } else {
    _message = std::move(other._message);
  }
}

error_base::error_base(const error_base& other)
    : _message(other._message), _file(other._file), _line(other._line) {
  if (other._caused_by != nullptr)
    _caused_by.reset(new error_base(*other._caused_by));
}

error_base::error_base(error_base&& other) noexcept = default;

error_base::~error_base() noexcept = default;

char const* error_base::what() const noexcept {
  return _message;  // TODO:MP what if null: caused_by
}

char const* error_base::msg() const noexcept {
  return _message;
}

uint32_t error_base::line() const noexcept {
  return _line;
}

}  // namespace internal

general_error<void>::general_error(const general_error& other) = default;

general_error<void>::general_error(general_error&& other) noexcept = default;

general_error<void>::~general_error() noexcept {}
}  // namespace rene