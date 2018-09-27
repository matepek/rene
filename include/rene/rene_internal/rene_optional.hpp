// JASL: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

//#include <type_traits>

#ifndef RENE_OPTIONAL
#  include <optional>
#  define RENE_OPTIONAL(T) std::optional<T>
#  define RENE_OPTIONAL_IN_PLACE_T std::in_place_t
#endif
#ifdef RENE_OPTINAL_INCLUDE
#  include RENE_OPTINAL_INCLUDE
#endif

namespace rene {
namespace internal {

template <typename T>
using optional = RENE_OPTIONAL(T);

using in_place_t = RENE_OPTIONAL_IN_PLACE_T;

// /*
//  * https://en.cppreference.com/w/cpp/utility/in_place
//  */
// struct in_place_t {
//   explicit in_place_t() = default;
// };
// inline constexpr in_place_t in_place{};

// /*
//  * This class was necessary because std::optional is kind of a new feature
//  and
//  * it isnt supported wide enough. Also, we save a byte with this solution.
//  (But
//  * it is not the reason.)
//  * WARNING! Easy to use this class wrongly.
//  * https://en.cppreference.com/w/cpp/types/aligned_storage
//  * https://en.cppreference.com/w/cpp/types/alignment_of
//  */
// template <typename T>
// class unsafe_optional {
//   static_assert(sizeof(T) > 0, "sizeof(T) > 0");
//   std::aligned_storage<sizeof(T), std::alignment_of<T>::value> _data;

//  public:
//   using value_type = T;

//   constexpr unsafe_optional() noexcept {}

//   template <typename... Args>

//   constexpr unsafe_optional(in_place_t, Args&& args) noexcept(
//       std::is_nothrow_constructible<T, Args...>::value) {
//     get_ptr()->T(std::forward<Args>(args)...);
//   }

//   void construct() {}

//   void destruct() noexcept(std::is_nothrow_destructible<T>::value) {
//     get_ptr()->~T();
//   }

//   const T& get() const& { return *get_ptr(); }
//   T& get() & { return *get_ptr(); }
//   T&& get() && { return std::move(*get_ptr()); }

//  private:
//   const T* get_ptr() const noexcept {
//     return reinterpret_cast<const T*>(&_data);
//   }

//   T* get_ptr() noexcept { return reinterpret_cast<T*>(&_data); }
// };

}  // namespace internal
}  // namespace rene
