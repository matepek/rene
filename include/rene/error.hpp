// JASL: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

#include <exception>
#include <memory>

#include "rene_internal/rene_common.hpp"
#include "rene_lib_export.hpp"

namespace rene {
namespace internal {

class result_base_no_type;

template <typename ErrT>
class result_base;

class RENE_EXPORT error_base : public std::exception {
  const char* _message;
  const char* _file;
  uint32_t _line;
  std::unique_ptr<error_base> _caused_by;

 protected:
  error_base(result_base_no_type&& result_with_caused_by,
             const char* file,
             uint32_t line) noexcept;

  error_base(const char* message, const char* file, uint32_t line) noexcept;
  error_base(error_base&& other, const char* file, uint32_t line);

  error_base(const error_base& other);
  error_base(error_base&& other) noexcept;

 public:
  ~error_base() noexcept;

  virtual char const* what() const noexcept override;

  const char* msg() const noexcept;
  uint32_t line() const noexcept;
};
}  // namespace internal

template <typename SpecialErrorCodeType>
class RENE_NODISCARD general_error final : public internal::error_base {
  static_assert(std::is_enum<SpecialErrorCodeType>::value,
                "Should be an enum!");
  SpecialErrorCodeType mErrorCode;

 public:
  general_error(internal::result_base<general_error>&& result_with_caused_by,
                const char* file,
                uint32_t line) noexcept
      : error_base(reinterpret_cast<internal::result_base_no_type&&>(
                       result_with_caused_by),
                   file,
                   line),
        mErrorCode(result_with_caused_by.get_error().mErrorCode) {}

  general_error(general_error&& other, const char* file, uint32_t line) noexcept
      : error_base(std::move(other), file, line),
        mErrorCode(std::move(other.mErrorCode)) {}

  general_error(SpecialErrorCodeType errorCode,
                const char* file,
                uint32_t line) noexcept
      : error_base(typeid(SpecialErrorCodeType).name() /*TODO:MP copy*/,
                   file,
                   line),
        mErrorCode(errorCode) {}

  general_error(const general_error& other) = default;
  general_error(general_error&& other) noexcept = default;

  virtual ~general_error() noexcept {}

  constexpr bool is(SpecialErrorCodeType errorCode) const noexcept {
    return errorCode == mErrorCode;
  }

  constexpr SpecialErrorCodeType get() const noexcept { return mErrorCode; }
};

template <>
class RENE_EXPORT RENE_NODISCARD general_error<void> final
    : public internal::error_base {
 public:
  general_error(internal::result_base<general_error>&& result_with_caused_by,
                const char* file,
                uint32_t line) noexcept
      : error_base(reinterpret_cast<internal::result_base_no_type&&>(
                       result_with_caused_by),
                   file,
                   line) {}

  general_error(general_error&& other, const char* file, uint32_t line) noexcept
      : error_base(std::move(other), file, line) {}

  general_error(const char* message, const char* file, uint32_t line)
      : error_base(message, file, line) {}

  general_error(const general_error& other);
  general_error(general_error&& other) noexcept;

  virtual ~general_error() noexcept;
};

using error = general_error<void>;

template <typename SpecErrorEnumT>
using spec_error = general_error<SpecErrorEnumT>;
}  // namespace rene

#define ERROR(message) ::rene::error(message, __FILE__, __LINE__)

#define SPEC_ERROR(value)                                                \
  ::rene::spec_error<std::decay<decltype(value)>::type>(value, __FILE__, \
                                                        __LINE__)
