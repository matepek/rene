// RENE: For more information see https://github.com/matepek/rene
//
// Copyright (c) 2018 Mate Pek
//
// This code is licensed under the MIT License (MIT).

#pragma once

/**
 * 1 or 0
 */
#ifndef RENE_SET_NODISCARD
#  ifdef __has_cpp_attribute
#    if __has_cpp_attribute(nodiscard)
#      define RENE_SET_NODISCARD 1
#    else
#      define RENE_SET_NODISCARD 0
#    endif
#  else
#    define RENE_SET_NODISCARD 0
#  endif
#endif

#if RENE_SET_NODISCARD
#  define RENE_NODISCARD [[nodiscard]]
#endif

// http://blog.man7.org/2012/10/how-much-do-builtinexpect-likely-and.html
#if defined(__GNUC__) && __GNUC__ >= 4
#  define _RENE_INTERNAL_LIKELY(...) (__builtin_expect(!!(__VA_ARGS__), 1))
#  define _RENE_INTERNAL_UNLIKELY(...) (__builtin_expect(!!(__VA_ARGS__), 0))
#else
#  define _RENE_INTERNAL_LIKELY(...) (__VA_ARGS__)
#  define _RENE_INTERNAL_UNLIKELY(...) (__VA_ARGS__)
#endif