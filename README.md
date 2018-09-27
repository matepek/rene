RENE (REsult aNd Error Library)
==================================

Error handling differently.

Influenced and inspired:
 * [rust library](https://doc.rust-lang.org/std/result/)
 * working experience

# Motivation

Sometimes we would like to use hardwired text resources. For example throwing an exception where the message is user specified. Just like in case of [std::runtime_error](http://en.cppreference.com/w/cpp/error/runtime_error) the message (_const char*_ parameter named *what_arg*) is "typically stored internally", which means the parameter will be copied. But usually users use it like `std::runtime_error("Coyote catched the Roadrunner!")` or `std::runtime_error(__FILE__)` where the *what_arg* is actually a a hardwired text. In this cases the copying seems unnecessary.

TODO

# Requirements

 * [C++ standard library](http://en.cppreference.com/w/cpp/header)
 * Minimum C++17
   * std::optional
 * [matepek/jasl](https://github.com/matepek/jasl) library's requirements:
   * clang, gcc or msvc
   * Minimum C++11:
     * _constexpr_ specifier
     * _noexcept_ specifier.
     * [std::hash](http://en.cppreference.com/w/cpp/utility/hash)
     * static_assert

# Installation

Include the headers, build and link the `cpp`-s.

# Documentation

TODO

# Configuration

TODO

# Changelog

[Here: CHANGELOG.md](CHANGELOG.md)

# Tests, Test Coverage and Performance Tests

## Requirements

## Building, testing and generating coverage report

### Tests

### Tested on

# Licence and third-parties

JASL library uses the [MIT](LICENSE) license.

# Thanks and Stuffs I used
 * NGG

# TODOs
 * TODO
