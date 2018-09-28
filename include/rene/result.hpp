// RENE: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

#include "rene/error.hpp"
#include "rene/general_result_macro.hpp"
#include "rene/general_result_with_variant.hpp"

namespace rene {

/**
 *
 */
template <typename... ResultTs>
using result = general_result_with_variant<error, ResultTs...>;

/**
 *
 */
template <typename SpecialErrorT, typename... ResultTs>
using spec_result =
    general_result_with_variant<spec_error<SpecialErrorT>, ResultTs...>;

}  // namespace rene
