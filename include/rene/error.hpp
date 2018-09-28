// RENE: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

#include "rene/general_error.hpp"

namespace rene {

/**
 *
 */
using error = general_error<void>;

/**
 *
 */
template <typename SpecErrorEnumT>
using spec_error = general_error<SpecErrorEnumT>;

}  // namespace rene

/**
 *
 */
#define RENE_ERROR(message) ::rene::error(message, __FILE__, __LINE__)

/**
 *
 */
#define RENE_SPEC_ERROR(value)                                           \
  ::rene::spec_error<std::decay<decltype(value)>::type>(value, __FILE__, \
                                                        __LINE__)
