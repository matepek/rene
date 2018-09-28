#include "catch2/catch.hpp"

#include <iostream>
#include <variant>

namespace rene {
namespace test {
namespace {

struct FooNoThrow {
  FooNoThrow() noexcept = default;
  FooNoThrow(const FooNoThrow&) noexcept = default;
  FooNoThrow(FooNoThrow&&) noexcept = default;
  FooNoThrow& operator=(const FooNoThrow&) noexcept = default;
  FooNoThrow& operator=(FooNoThrow&&) noexcept = default;
  ~FooNoThrow() noexcept = default;
};

struct FooThrow {
  FooThrow() noexcept(false) {}
  FooThrow(const FooThrow&) noexcept(false) {}
  FooThrow(FooThrow&&) noexcept(false) {}
  FooThrow& operator=(const FooThrow&) noexcept(false) { return *this; }
  FooThrow& operator=(FooThrow&&) noexcept(false) { return *this; }
  ~FooThrow() noexcept(false) {}
};

enum class TErrCode { Err1, Err2 };
using TError = spec_error<TErrCode>;

// clang-format off

// Checking type_traits
namespace {

using namespace std;

static_assert(is_constructible_v<std::optional<FooThrow>, std::nullopt_t>);
//static_assert(is_nothrow_constructible_v<std::optional<FooThrow>, std::nullopt_t>);
static_assert(is_constructible_v<std::optional<FooThrow>>);
//static_assert(is_nothrow_constructible_v<std::optional<FooThrow>>);

static_assert(is_nothrow_constructible_v<tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_constructible_v<tuple<FooNoThrow, FooNoThrow>, const FooNoThrow&, const FooNoThrow&>);
static_assert(is_nothrow_constructible_v<tuple<FooNoThrow, FooNoThrow>, FooNoThrow&&, FooNoThrow&&>);
static_assert(is_nothrow_copy_constructible_v<tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_move_constructible_v<tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_copy_assignable_v<tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_move_assignable_v<tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_assignable_v<tuple<FooNoThrow, FooNoThrow>, const tuple<FooNoThrow, FooNoThrow>&>);
static_assert(is_nothrow_assignable_v<tuple<FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>&&>);
static_assert(is_nothrow_destructible_v<tuple<FooNoThrow, FooNoThrow>>);

///

static_assert(!is_nothrow_constructible_v<tuple<FooThrow, FooThrow>>);
static_assert(!is_nothrow_constructible_v<tuple<FooThrow, FooThrow>, const FooThrow&, const FooThrow&>);
static_assert(!is_nothrow_constructible_v<tuple<FooThrow, FooThrow>, FooThrow&&, FooThrow&&>);
static_assert(!is_nothrow_copy_constructible_v<tuple<FooThrow, FooThrow>>);
static_assert(!is_nothrow_move_constructible_v<tuple<FooThrow, FooThrow>>);
static_assert(!is_nothrow_copy_assignable_v<tuple<FooThrow, FooThrow>>);
static_assert(!is_nothrow_move_assignable_v<tuple<FooThrow, FooThrow>>);
static_assert(!is_nothrow_assignable_v<tuple<FooThrow, FooThrow>, const tuple<FooThrow, FooThrow>&>);
static_assert(!is_nothrow_assignable_v<tuple<FooThrow, FooThrow>, tuple<FooThrow, FooThrow>&&>);
static_assert(!is_nothrow_destructible_v<tuple<FooThrow, FooThrow>>);

///

static_assert(!is_nothrow_constructible_v<tuple<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_constructible_v<tuple<FooNoThrow, FooThrow>, const FooNoThrow&, const FooThrow&>);
static_assert(!is_nothrow_constructible_v<tuple<FooNoThrow, FooThrow>, FooNoThrow&&, FooThrow&&>);
static_assert(!is_nothrow_copy_constructible_v<tuple<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_move_constructible_v<tuple<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_copy_assignable_v<tuple<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_move_assignable_v<tuple<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_assignable_v<tuple<FooNoThrow, FooThrow>, const tuple<FooNoThrow, FooThrow>&>);
static_assert(!is_nothrow_assignable_v<tuple<FooNoThrow, FooThrow>, tuple<FooNoThrow, FooThrow>&&>);
static_assert(!is_nothrow_destructible_v<tuple<FooNoThrow, FooThrow>>);

}  // namespace

namespace {

using namespace std;

static_assert(is_constructible_v<result<>>);
static_assert(is_nothrow_constructible_v<result<>>);
static_assert(is_constructible_v<result<>, error>);
static_assert(!is_nothrow_constructible_v<result<>, error>);
static_assert(is_constructible_v<result<>, unique_ptr<error>>);
static_assert(is_nothrow_constructible_v<result<>, unique_ptr<error>>);
static_assert(!is_copy_constructible_v<result<>>);
static_assert(!is_nothrow_copy_constructible_v<result<>>);
static_assert(is_move_constructible_v<result<>>);
static_assert(is_nothrow_move_constructible_v<result<>>);
static_assert(!is_copy_assignable_v<result<>>);
static_assert(!is_nothrow_copy_assignable_v<result<>>);
static_assert(is_move_assignable_v<result<>>);
static_assert(is_nothrow_move_assignable_v<result<>>);
static_assert(is_assignable_v<result<>, error>);
static_assert(!is_nothrow_assignable_v<result<>, error>);
static_assert(is_assignable_v<result<>, unique_ptr<error>>);
static_assert(is_nothrow_assignable_v<result<>, unique_ptr<error>>);
static_assert(is_destructible_v<result<>>);
static_assert(is_nothrow_destructible_v<result<>>);

//

static_assert(is_constructible_v<result<FooNoThrow>, FooNoThrow>);
static_assert(is_nothrow_constructible_v<result<FooNoThrow>, FooNoThrow>);
static_assert(is_constructible_v<result<FooNoThrow>, const FooNoThrow&>);
static_assert(is_nothrow_constructible_v<result<FooNoThrow>, const FooNoThrow&>);
static_assert(is_constructible_v<result<FooNoThrow>, FooNoThrow&&>);
static_assert(is_nothrow_constructible_v<result<FooNoThrow>, FooNoThrow&&>);
static_assert(is_constructible_v<result<FooNoThrow>, error>);
static_assert(!is_nothrow_constructible_v<result<FooNoThrow>, error>);
static_assert(is_constructible_v<result<FooNoThrow>, unique_ptr<error>>);
static_assert(is_nothrow_constructible_v<result<FooNoThrow>, unique_ptr<error>>);
static_assert(!is_copy_constructible_v<result<FooNoThrow>>);
static_assert(!is_nothrow_copy_constructible_v<result<FooNoThrow>>);
static_assert(is_move_constructible_v<result<FooNoThrow>>);
static_assert(is_nothrow_move_constructible_v<result<FooNoThrow>>);
static_assert(!is_copy_assignable_v<result<FooNoThrow>>);
static_assert(!is_nothrow_copy_assignable_v<result<FooNoThrow>>);
static_assert(is_move_assignable_v<result<FooNoThrow>>);
static_assert(is_nothrow_move_assignable_v<result<FooNoThrow>>);
static_assert(is_assignable_v<result<FooNoThrow>, FooNoThrow>);
static_assert(is_nothrow_assignable_v<result<FooNoThrow>, FooNoThrow>);
static_assert(is_assignable_v<result<FooNoThrow>, const FooNoThrow&>);
static_assert(is_nothrow_assignable_v<result<FooNoThrow>, const FooNoThrow&>);
static_assert(is_assignable_v<result<FooNoThrow>, FooNoThrow&&>);
static_assert(is_nothrow_assignable_v<result<FooNoThrow>, FooNoThrow&&>);
static_assert(is_assignable_v<result<FooNoThrow>, error>);
static_assert(!is_nothrow_assignable_v<result<FooNoThrow>, error>);
static_assert(is_assignable_v<result<FooNoThrow>, unique_ptr<error>>);
static_assert(is_nothrow_assignable_v<result<FooNoThrow>, unique_ptr<error>>);
static_assert(is_destructible_v<result<FooNoThrow>>);
static_assert(is_nothrow_destructible_v<result<FooNoThrow>>);

static_assert(is_constructible_v<result<FooThrow>, FooThrow>);
static_assert(!is_nothrow_constructible_v<result<FooThrow>, FooThrow>);
static_assert(is_constructible_v<result<FooThrow>, const FooThrow&>);
static_assert(!is_nothrow_constructible_v<result<FooThrow>, const FooThrow&>);
static_assert(is_constructible_v<result<FooThrow>, FooThrow&&>);
static_assert(!is_nothrow_constructible_v<result<FooThrow>, FooThrow&&>);
static_assert(is_constructible_v<result<FooThrow>, error>);
static_assert(!is_nothrow_constructible_v<result<FooThrow>, error>);
static_assert(is_constructible_v<result<FooThrow>, unique_ptr<error>>);
#warning "bug: libc++ https://bugs.llvm.org/show_bug.cgi?id=39175"
//static_assert(is_nothrow_constructible_v<result<FooThrow>, unique_ptr<error>>);
// --> static_assert(noexcept(optional<FooThrow>()));
static_assert(!is_copy_constructible_v<result<FooThrow>>);
static_assert(!is_nothrow_copy_constructible_v<result<FooThrow>>);
static_assert(is_move_constructible_v<result<FooThrow>>);
static_assert(!is_nothrow_move_constructible_v<result<FooThrow>>);
static_assert(!is_copy_assignable_v<result<FooThrow>>);
static_assert(!is_nothrow_copy_assignable_v<result<FooThrow>>);
static_assert(is_move_assignable_v<result<FooThrow>>);
static_assert(!is_nothrow_move_assignable_v<result<FooThrow>>);
static_assert(is_assignable_v<result<FooThrow>, FooThrow>);
static_assert(!is_nothrow_assignable_v<result<FooThrow>, FooThrow>);
static_assert(is_assignable_v<result<FooThrow>, const FooThrow&>);
static_assert(!is_nothrow_assignable_v<result<FooThrow>, const FooThrow&>);
static_assert(is_assignable_v<result<FooThrow>, FooThrow&&>);
static_assert(!is_nothrow_assignable_v<result<FooThrow>, FooThrow&&>);
static_assert(is_assignable_v<result<FooThrow>, error>);
static_assert(!is_nothrow_assignable_v<result<FooThrow>, error>);
static_assert(is_assignable_v<result<FooThrow>, unique_ptr<error>>);
static_assert(!is_nothrow_assignable_v<result<FooThrow>, unique_ptr<error>>);
static_assert(is_destructible_v<result<FooThrow>>);
static_assert(!is_nothrow_destructible_v<result<FooThrow>>);

//

static_assert(is_constructible_v<result<FooNoThrow, FooNoThrow>, FooNoThrow, FooNoThrow>);
static_assert(is_nothrow_constructible_v<result<FooNoThrow, FooNoThrow>, FooNoThrow, FooNoThrow>);
static_assert(is_constructible_v<result<FooNoThrow, FooNoThrow>, const FooNoThrow&, const FooNoThrow&>);
static_assert(is_nothrow_constructible_v<result<FooNoThrow, FooNoThrow>, const FooNoThrow&, const FooNoThrow&>);
static_assert(is_constructible_v<result<FooNoThrow, FooNoThrow>, FooNoThrow&&, FooNoThrow&&>);
static_assert(is_nothrow_constructible_v<result<FooNoThrow, FooNoThrow>, FooNoThrow&&, FooNoThrow&&>);
static_assert(is_constructible_v<result<FooNoThrow, FooNoThrow>, error>);
static_assert(!is_nothrow_constructible_v<result<FooNoThrow, FooNoThrow>, error>);
static_assert(is_constructible_v<result<FooNoThrow, FooNoThrow>, unique_ptr<error>>);
static_assert(is_nothrow_constructible_v<result<FooNoThrow, FooNoThrow>, unique_ptr<error>>);
static_assert(!is_copy_constructible_v<result<FooNoThrow, FooNoThrow>>);
static_assert(!is_nothrow_copy_constructible_v<result<FooNoThrow, FooNoThrow>>);
static_assert(is_move_constructible_v<result<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_move_constructible_v<result<FooNoThrow, FooNoThrow>>);
static_assert(!is_copy_assignable_v<result<FooNoThrow, FooNoThrow>>);
static_assert(!is_nothrow_copy_assignable_v<result<FooNoThrow, FooNoThrow>>);
static_assert(is_move_assignable_v<result<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_move_assignable_v<result<FooNoThrow, FooNoThrow>>);
static_assert(is_assignable_v<result<FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_assignable_v<result<FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_assignable_v<result<FooNoThrow, FooNoThrow>, const tuple<FooNoThrow, FooNoThrow>&>);
static_assert(is_nothrow_assignable_v<result<FooNoThrow, FooNoThrow>, const tuple<FooNoThrow, FooNoThrow>&>);
static_assert(is_assignable_v<result<FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>&&>);
static_assert(is_nothrow_assignable_v<result<FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>&&>);
static_assert(is_assignable_v<result<FooNoThrow, FooNoThrow>, error>);
static_assert(!is_nothrow_assignable_v<result<FooNoThrow, FooNoThrow>, error>);
static_assert(is_assignable_v<result<FooNoThrow, FooNoThrow>, unique_ptr<error>>);
static_assert(is_nothrow_assignable_v<result<FooNoThrow, FooNoThrow>, unique_ptr<error>>);
static_assert(is_destructible_v<result<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_destructible_v<result<FooNoThrow, FooNoThrow>>);

static_assert(is_constructible_v<result<FooNoThrow, FooThrow>, FooNoThrow, FooThrow>);
static_assert(!is_nothrow_constructible_v<result<FooNoThrow, FooThrow>, FooNoThrow, FooThrow>);
static_assert(is_constructible_v<result<FooNoThrow, FooThrow>, const FooNoThrow&, const FooThrow&>);
static_assert(!is_nothrow_constructible_v<result<FooNoThrow, FooThrow>, const FooNoThrow&, const FooThrow&>);
static_assert(is_constructible_v<result<FooNoThrow, FooThrow>, FooNoThrow&&, FooThrow&&>);
static_assert(!is_nothrow_constructible_v<result<FooNoThrow, FooThrow>, FooNoThrow&&, FooThrow&&>);
static_assert(is_constructible_v<result<FooNoThrow, FooThrow>, error>);
static_assert(!is_nothrow_constructible_v<result<FooNoThrow, FooThrow>, error>);
static_assert(is_constructible_v<result<FooNoThrow, FooThrow>, unique_ptr<error>>);
#warning "bug: libc++ https://bugs.llvm.org/show_bug.cgi?id=39175"
//static_assert(is_nothrow_constructible_v<result<FooNoThrow, FooThrow>, unique_ptr<error>>);
static_assert(!is_copy_constructible_v<result<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_copy_constructible_v<result<FooNoThrow, FooThrow>>);
static_assert(is_move_constructible_v<result<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_move_constructible_v<result<FooNoThrow, FooThrow>>);
static_assert(!is_copy_assignable_v<result<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_copy_assignable_v<result<FooNoThrow, FooThrow>>);
static_assert(is_move_assignable_v<result<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_move_assignable_v<result<FooNoThrow, FooThrow>>);
static_assert(is_assignable_v<result<FooNoThrow, FooThrow>, tuple<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_assignable_v<result<FooNoThrow, FooThrow>, tuple<FooNoThrow, FooThrow>>);
static_assert(is_assignable_v<result<FooNoThrow, FooThrow>, const tuple<FooNoThrow, FooThrow>&>);
static_assert(!is_nothrow_assignable_v<result<FooNoThrow, FooThrow>, const tuple<FooNoThrow, FooThrow>&>);
static_assert(is_assignable_v<result<FooNoThrow, FooThrow>, tuple<FooNoThrow, FooThrow>&&>);
static_assert(!is_nothrow_assignable_v<result<FooNoThrow, FooThrow>, tuple<FooNoThrow, FooThrow>&&>);
static_assert(is_assignable_v<result<FooNoThrow, FooThrow>, error>);
static_assert(!is_nothrow_assignable_v<result<FooNoThrow, FooThrow>, error>);
static_assert(is_assignable_v<result<FooNoThrow, FooThrow>, unique_ptr<error>>);
static_assert(!is_nothrow_assignable_v<result<FooNoThrow, FooThrow>, unique_ptr<error>>);
static_assert(is_destructible_v<result<FooNoThrow, FooThrow>>);
static_assert(!is_nothrow_destructible_v<result<FooNoThrow, FooThrow>>);

///

static_assert(is_constructible_v<spec_result<TErrCode>>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode>>);
static_assert(is_constructible_v<spec_result<TErrCode>, spec_error<TErrCode>>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode>, spec_error<TErrCode>>);
static_assert(is_constructible_v<spec_result<TErrCode>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode>, unique_ptr<spec_error<TErrCode>>>);
static_assert(!is_copy_constructible_v<spec_result<TErrCode>>);
static_assert(!is_nothrow_copy_constructible_v<spec_result<TErrCode>>);
static_assert(is_move_constructible_v<spec_result<TErrCode>>);
static_assert(is_nothrow_move_constructible_v<spec_result<TErrCode>>);
static_assert(!is_copy_assignable_v<spec_result<TErrCode>>);
static_assert(!is_nothrow_copy_assignable_v<spec_result<TErrCode>>);
static_assert(is_move_assignable_v<spec_result<TErrCode>>);
static_assert(is_nothrow_move_assignable_v<spec_result<TErrCode>>);
static_assert(is_assignable_v<spec_result<TErrCode>, spec_error<TErrCode>>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode>, spec_error<TErrCode>>);
static_assert(is_assignable_v<spec_result<TErrCode>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_destructible_v<spec_result<TErrCode>>);
static_assert(is_nothrow_destructible_v<spec_result<TErrCode>>);

//

static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow>, FooNoThrow>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow>, FooNoThrow>);
static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow>, const FooNoThrow&>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow>, const FooNoThrow&>);
static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow>, FooNoThrow&&>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow>, FooNoThrow&&>);
static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow>, spec_error<TErrCode>>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow>, spec_error<TErrCode>>);
static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(!is_copy_constructible_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(!is_nothrow_copy_constructible_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(is_move_constructible_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(is_nothrow_move_constructible_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(!is_copy_assignable_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(!is_nothrow_copy_assignable_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(is_move_assignable_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(is_nothrow_move_assignable_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow>, FooNoThrow>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow>, FooNoThrow>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow>, const FooNoThrow&>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow>, const FooNoThrow&>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow>, FooNoThrow&&>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow>, FooNoThrow&&>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow>, spec_error<TErrCode>>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow>, spec_error<TErrCode>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_destructible_v<spec_result<TErrCode, FooNoThrow>>);
static_assert(is_nothrow_destructible_v<spec_result<TErrCode, FooNoThrow>>);

static_assert(is_constructible_v<spec_result<TErrCode, FooThrow>, FooThrow>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooThrow>, FooThrow>);
static_assert(is_constructible_v<spec_result<TErrCode, FooThrow>, const FooThrow&>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooThrow>, const FooThrow&>);
static_assert(is_constructible_v<spec_result<TErrCode, FooThrow>, FooThrow&&>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooThrow>, FooThrow&&>);
static_assert(is_constructible_v<spec_result<TErrCode, FooThrow>, spec_error<TErrCode>>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooThrow>, spec_error<TErrCode>>);
static_assert(is_constructible_v<spec_result<TErrCode, FooThrow>, unique_ptr<spec_error<TErrCode>>>);
#warning "bug: libc++ https://bugs.llvm.org/show_bug.cgi?id=39175"
//static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(!is_copy_constructible_v<spec_result<TErrCode, FooThrow>>);
static_assert(!is_nothrow_copy_constructible_v<spec_result<TErrCode, FooThrow>>);
static_assert(is_move_constructible_v<spec_result<TErrCode, FooThrow>>);
static_assert(!is_nothrow_move_constructible_v<spec_result<TErrCode, FooThrow>>);
static_assert(!is_copy_assignable_v<spec_result<TErrCode, FooThrow>>);
static_assert(!is_nothrow_copy_assignable_v<spec_result<TErrCode, FooThrow>>);
static_assert(is_move_assignable_v<spec_result<TErrCode, FooThrow>>);
static_assert(!is_nothrow_move_assignable_v<spec_result<TErrCode, FooThrow>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow>, FooThrow>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow>, FooThrow>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow>, const FooThrow&>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow>, const FooThrow&>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow>, FooThrow&&>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow>, FooThrow&&>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow>, spec_error<TErrCode>>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow>, spec_error<TErrCode>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_destructible_v<spec_result<TErrCode, FooThrow>>);
static_assert(!is_nothrow_destructible_v<spec_result<TErrCode, FooThrow>>);

//

static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, FooNoThrow, FooNoThrow>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, FooNoThrow, FooNoThrow>);
static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, const FooNoThrow&, const FooNoThrow&>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, const FooNoThrow&, const FooNoThrow&>);
static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, FooNoThrow&&, FooNoThrow&&>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, FooNoThrow&&, FooNoThrow&&>);
static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, spec_error<TErrCode>>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, spec_error<TErrCode>>);
static_assert(is_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(!is_copy_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(!is_nothrow_copy_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(is_move_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_move_constructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(!is_copy_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(!is_nothrow_copy_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(is_move_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_move_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, const tuple<FooNoThrow, FooNoThrow>&>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, const tuple<FooNoThrow, FooNoThrow>&>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>&&>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, tuple<FooNoThrow, FooNoThrow>&&>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, spec_error<TErrCode>>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, spec_error<TErrCode>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_nothrow_assignable_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_destructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);
static_assert(is_nothrow_destructible_v<spec_result<TErrCode, FooNoThrow, FooNoThrow>>);

static_assert(is_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, FooThrow, FooNoThrow>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, FooThrow, FooNoThrow>);
static_assert(is_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, const FooThrow&, const FooNoThrow&>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, const FooThrow&, const FooNoThrow&>);
static_assert(is_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, FooThrow&&, FooNoThrow&&>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, FooThrow&&, FooNoThrow&&>);
static_assert(is_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, spec_error<TErrCode>>);
static_assert(!is_nothrow_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, spec_error<TErrCode>>);
static_assert(is_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
#warning "bug: libc++ https://bugs.llvm.org/show_bug.cgi?id=39175"
//static_assert(is_nothrow_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(!is_copy_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(!is_nothrow_copy_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(is_move_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(!is_nothrow_move_constructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(!is_copy_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(!is_nothrow_copy_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(is_move_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(!is_nothrow_move_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, tuple<FooThrow, FooNoThrow>>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, tuple<FooThrow, FooNoThrow>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, const tuple<FooThrow, FooNoThrow>&>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, const tuple<FooThrow, FooNoThrow>&>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, tuple<FooThrow, FooNoThrow>&&>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, tuple<FooThrow, FooNoThrow>&&>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, spec_error<TErrCode>>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, spec_error<TErrCode>>);
static_assert(is_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(!is_nothrow_assignable_v<spec_result<TErrCode, FooThrow, FooNoThrow>, unique_ptr<spec_error<TErrCode>>>);
static_assert(is_destructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);
static_assert(!is_nothrow_destructible_v<spec_result<TErrCode, FooThrow, FooNoThrow>>);

}  // namespace

// clang-format on

TEST_CASE(RENE_TEST_PREFIX "result<>") {
  {
    auto func = []() -> result<> { return {}; };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
    }
    auto func_return_func = [&]() -> result<> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
  }
  {
    auto func = []() -> result<> { return result<>(); };
    auto res = func();
    REQUIRE(res.is_error() == false);
  }
  {
    auto func = []() -> result<> {
      result<> res;
      return res;
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
  }
  const char errMsg[] = "TestResultErrorMsg";
  {
    auto func = [&]() -> result<> { return error(errMsg, __FILE__, __LINE__); };
    {
      auto res = func();
      REQUIRE(res.is_error() == true);
      REQUIRE(res.get_error().msg() == std::string(errMsg));
    }
    auto func_return_func = [&]() -> result<> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().msg() == std::string(errMsg));
  }
  {
    auto func = [&]() -> result<> {
      return result<>(error(errMsg, __FILE__, __LINE__));
    };
    auto res = func();
    REQUIRE(res.is_error() == true);
  }
}

result<> retunEmptyResultOrError(bool initWithError, bool returnWithError) {
  if (initWithError) {
    result<> res(error("retunEmptyResultOrError", __FILE__, __LINE__));
    if (!returnWithError)
      res = {};
    return res;
  } else {
    result<> res;
    if (returnWithError)
      res = error("retunEmptyResultOrError", __FILE__, __LINE__);
    return res;
  }
}

TEST_CASE(RENE_TEST_PREFIX "retunEmptyResultOrError") {
  REQUIRE(retunEmptyResultOrError(true, false).is_error() == false);
  REQUIRE(retunEmptyResultOrError(false, false).is_error() == false);
  REQUIRE(retunEmptyResultOrError(true, true).is_error() == true);
  REQUIRE(retunEmptyResultOrError(false, true).is_error() == true);
}

TEST_CASE(RENE_TEST_PREFIX "result<int>") {
  {
    auto func = []() -> result<int> { return 3; };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
      REQUIRE(res.get() == 3);
    }
    auto func_return_func = [&]() -> result<int> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto func = []() -> result<int> { return result<int>(3); };
    auto res = func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto func = []() -> result<int> {
      result<int> res(3);
      return res;
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  const char errMsg[] = "TestResultErrorMsg";
  {
    auto func = [&]() -> result<int> {
      return error(errMsg, __FILE__, __LINE__);
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == true);
      REQUIRE(res.get_error().msg() == std::string(errMsg));
    }
    auto func_return_func = [&]() -> result<int> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().msg() == std::string(errMsg));
  }
  {
    auto func = [&]() -> result<int> {
      return result<int>(error(errMsg, __FILE__, __LINE__));
    };
    auto res = func();
    REQUIRE(res.is_error() == true);
  }
}

result<int> retunIntResultOrError(bool initWithError, bool returnWithError) {
  if (initWithError) {
    result<int> res(error("retunIntResultOrError", __FILE__, __LINE__));
    if (!returnWithError)
      res = 3;
    return res;
  } else {
    result<int> res(3);
    if (returnWithError)
      res = error("retunIntResultOrError", __FILE__, __LINE__);
    return res;
  }
}

TEST_CASE(RENE_TEST_PREFIX "retunIntResultOrError") {
  {
    auto res = retunIntResultOrError(true, false);
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto res = retunIntResultOrError(false, false);
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto res = retunIntResultOrError(true, true);
    REQUIRE(res.is_error() == true);
  }
  {
    auto res = retunIntResultOrError(false, true);
    REQUIRE(res.is_error() == true);
  }
}

TEST_CASE(RENE_TEST_PREFIX "result<int,bool>") {
  {
    result<int, bool> a({3, true});
    REQUIRE_FALSE(a.is_error());
    REQUIRE(a.get<0>() == 3);
    REQUIRE(a.get<1>() == true);
  }
  {
    result<int, bool> a(3, true);
    REQUIRE_FALSE(a.is_error());
    REQUIRE(a.get<0>() == 3);
    REQUIRE(a.get<1>() == true);
  }
  {
    result<int, bool> a{3, true};
    REQUIRE_FALSE(a.is_error());
    REQUIRE(a.get<0>() == 3);
    REQUIRE(a.get<1>() == true);

    a = std::make_tuple(4, false);
    REQUIRE_FALSE(a.is_error());
    REQUIRE(a.get<0>() == 4);
    REQUIRE(a.get<1>() == false);
  }
  {
    result<int, bool> a{error("", "", 1)};
    REQUIRE(a.is_error());
    REQUIRE(a.get_error().line() == 1);
  }
  {
    result<int, bool> a(error("", "", 2));
    REQUIRE(a.is_error());
    REQUIRE(a.get_error().line() == 2);

    a = error("", "", 3);
    REQUIRE(a.get_error().line() == 3);
  }
  []() -> result<int, bool> { return result<int, bool>(3, true); }();
  []() -> result<int, bool> { return {3, true}; }();
  []() -> result<int, bool> { return error("", "", 1); }();
  {
    auto func = []() -> result<int, bool> {
      return result<int, bool>({3, true});
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
      REQUIRE(res.get<0>() == 3);
      REQUIRE(res.get<1>() == true);
    }
    auto func_return_func = [&]() -> result<int, bool> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto func = []() -> result<int, bool> {
      return result<int, bool>({3, true});
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto func = []() -> result<int, bool> {
      result<int, bool> res({3, true});
      return res;
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  const char errMsg[] = "TestResultErrorMsg";
  {
    auto func = [&]() -> result<int, bool> {
      return error(errMsg, __FILE__, __LINE__);
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == true);
      REQUIRE(res.get_error().msg() == std::string(errMsg));
    }
    auto func_return_func = [&]() -> result<int, bool> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().msg() == std::string(errMsg));
  }
  {
    auto func = [&]() -> result<int, bool> {
      return result<int, bool>(error(errMsg, __FILE__, __LINE__));
    };
    auto res = func();
    REQUIRE(res.is_error() == true);
  }
}

result<int, bool> retunIntBoolResultOrError(bool initWithError,
                                            bool returnWithError) {
  if (initWithError) {
    result<int, bool> res(
        error("retunIntBoolResultOrError", __FILE__, __LINE__));
    if (!returnWithError)
      res = std::make_tuple(3, true);
    return res;
  } else {
    result<int, bool> res{3, true};
    if (returnWithError)
      res = error("retunIntBoolResultOrError", __FILE__, __LINE__);
    return res;
  }
}

TEST_CASE(RENE_TEST_PREFIX "retunIntBoolResultOrError") {
  {
    auto res = retunIntBoolResultOrError(true, false);
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto res = retunIntBoolResultOrError(false, false);
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto res = retunIntBoolResultOrError(true, true);
    REQUIRE(res.is_error() == true);
  }
  {
    auto res = retunIntBoolResultOrError(false, true);
    REQUIRE(res.is_error() == true);
  }
}

TEST_CASE(RENE_TEST_PREFIX "spec_result<TErrCode>") {
  {
    auto func = []() -> spec_result<TErrCode> { return {}; };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
    }
    auto func_return_func = [&]() -> spec_result<TErrCode> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
  }
  {
    auto func = []() -> spec_result<TErrCode> {
      return spec_result<TErrCode>();
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
  }
  {
    auto func = []() -> spec_result<TErrCode> {
      spec_result<TErrCode> res;
      return res;
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
  }
  {
    auto func = [&]() -> spec_result<TErrCode> {
      return TError(TErrCode::Err1, __FILE__, __LINE__);
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == true);
      REQUIRE(res.get_error().get() == TErrCode::Err1);
    }
    auto func_return_func = [&]() -> spec_result<TErrCode> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
  {
    auto func = [&]() -> spec_result<TErrCode> {
      return spec_result<TErrCode>(TError(TErrCode::Err1, __FILE__, __LINE__));
    };
    auto res = func();
    REQUIRE(res.is_error() == true);
  }
}

spec_result<TErrCode> retunEmptyResultOrSpecError(bool initWithError,
                                                  bool returnWithError) {
  if (initWithError) {
    spec_result<TErrCode> res(TError(TErrCode::Err1, __FILE__, __LINE__));
    if (!returnWithError)
      res = {};
    return res;
  } else {
    spec_result<TErrCode> res;
    if (returnWithError)
      res = TError(TErrCode::Err1, __FILE__, __LINE__);
    return res;
  }
}

TEST_CASE(RENE_TEST_PREFIX "retunEmptyResultOrSpecError") {
  {
    auto res = retunEmptyResultOrSpecError(true, false);
    REQUIRE(res.is_error() == false);
  }
  {
    auto res = retunEmptyResultOrSpecError(false, false);
    REQUIRE(res.is_error() == false);
  }
  {
    auto res = retunEmptyResultOrSpecError(true, true);
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
  {
    auto res = retunEmptyResultOrSpecError(false, true);
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
}

TEST_CASE(RENE_TEST_PREFIX "spec_result<TErrCode, int>") {
  {
    auto func = []() -> spec_result<TErrCode, int> { return 3; };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
      REQUIRE(res.get() == 3);
    }
    auto func_return_func = [&]() -> spec_result<TErrCode, int> {
      return func();
    };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto func = []() -> spec_result<TErrCode, int> {
      return spec_result<TErrCode, int>(3);
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto func = []() -> spec_result<TErrCode, int> {
      spec_result<TErrCode, int> res(3);
      return res;
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto func = [&]() -> spec_result<TErrCode, int> {
      return TError(TErrCode::Err1, __FILE__, __LINE__);
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == true);
      REQUIRE(res.get_error().get() == TErrCode::Err1);
    }
    auto func_return_func = [&]() -> spec_result<TErrCode, int> {
      return func();
    };
    auto res = func_return_func();
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
  {
    auto func = [&]() -> spec_result<TErrCode, int> {
      return spec_result<TErrCode, int>(
          TError(TErrCode::Err1, __FILE__, __LINE__));
    };
    auto res = func();
    REQUIRE(res.is_error() == true);
  }
}

spec_result<TErrCode, int> retunIntResultOrSpecError(bool initWithError,
                                                     bool returnWithError) {
  if (initWithError) {
    spec_result<TErrCode, int> res(TError(TErrCode::Err1, __FILE__, __LINE__));
    if (!returnWithError)
      res = 3;
    return res;
  } else {
    spec_result<TErrCode, int> res(3);
    if (returnWithError)
      res = TError(TErrCode::Err1, __FILE__, __LINE__);
    return res;
  }
}

TEST_CASE(RENE_TEST_PREFIX "retunIntResultOrSpecError") {
  {
    auto res = retunIntResultOrSpecError(true, false);
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto res = retunIntResultOrSpecError(false, false);
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get() == 3);
  }
  {
    auto res = retunIntResultOrSpecError(true, true);
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
  {
    auto res = retunIntResultOrSpecError(false, true);
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
}

TEST_CASE(RENE_TEST_PREFIX "spec_result<TErrCode,int,bool>") {
  {
    spec_result<TErrCode, int, bool> a({3, true});
    REQUIRE_FALSE(a.is_error());
    REQUIRE(a.get<0>() == 3);
    REQUIRE(a.get<1>() == true);
  }
  {
    spec_result<TErrCode, int, bool> a(3, true);
    REQUIRE_FALSE(a.is_error());
    REQUIRE(a.get<0>() == 3);
    REQUIRE(a.get<1>() == true);
  }
  {
    spec_result<TErrCode, int, bool> a{3, true};
    REQUIRE_FALSE(a.is_error());
    REQUIRE(a.get<0>() == 3);
    REQUIRE(a.get<1>() == true);

    a = std::make_tuple(4, false);
    REQUIRE_FALSE(a.is_error());
    REQUIRE(a.get<0>() == 4);
    REQUIRE(a.get<1>() == false);
  }
  {
    spec_result<TErrCode, int, bool> a{TError(TErrCode::Err1, "", 1)};
    REQUIRE(a.is_error());
    REQUIRE(a.get_error().line() == 1);
    REQUIRE(a.get_error().get() == TErrCode::Err1);
  }
  {
    spec_result<TErrCode, int, bool> a(TError(TErrCode::Err1, "", 2));
    REQUIRE(a.is_error());
    REQUIRE(a.get_error().line() == 2);
    REQUIRE(a.get_error().get() == TErrCode::Err1);

    a = TError(TErrCode::Err2, "", 3);
    REQUIRE(a.get_error().line() == 3);
    REQUIRE(a.get_error().get() == TErrCode::Err2);
  }
  []() -> spec_result<TErrCode, int, bool> {
    return spec_result<TErrCode, int, bool>(3, true);
  }();
  []() -> spec_result<TErrCode, int, bool> { return {3, true}; }();
  []() -> spec_result<TErrCode, int, bool> {
    return TError(TErrCode::Err1, "", 1);
  }();
  {
    auto func = []() -> spec_result<TErrCode, int, bool> {
      return spec_result<TErrCode, int, bool>({3, true});
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
      REQUIRE(res.get<0>() == 3);
      REQUIRE(res.get<1>() == true);
    }
    auto func_return_func = [&]() -> spec_result<TErrCode, int, bool> {
      return func();
    };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto func = []() -> spec_result<TErrCode, int, bool> {
      return spec_result<TErrCode, int, bool>({3, true});
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto func = []() -> spec_result<TErrCode, int, bool> {
      spec_result<TErrCode, int, bool> res({3, true});
      return res;
    };
    auto res = func();
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto func = [&]() -> spec_result<TErrCode, int, bool> {
      return TError(TErrCode::Err1, __FILE__, __LINE__);
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == true);
      REQUIRE(res.get_error().get() == TErrCode::Err1);
    }
    auto func_return_func = [&]() -> spec_result<TErrCode, int, bool> {
      return func();
    };
    auto res = func_return_func();
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
  {
    auto func = [&]() -> spec_result<TErrCode, int, bool> {
      return spec_result<TErrCode, int, bool>(
          TError(TErrCode::Err1, __FILE__, __LINE__));
    };
    auto res = func();
    REQUIRE(res.is_error() == true);
  }
}

spec_result<TErrCode, int, bool> retunIntBoolResultOrSpecError(
    bool initWithError,
    bool returnWithError) {
  if (initWithError) {
    spec_result<TErrCode, int, bool> res(
        TError(TErrCode::Err1, __FILE__, __LINE__));
    if (!returnWithError)
      res = std::make_tuple(3, true);
    return res;
  } else {
    spec_result<TErrCode, int, bool> res{3, true};
    if (returnWithError)
      res = TError(TErrCode::Err1, __FILE__, __LINE__);
    return res;
  }
}

TEST_CASE(RENE_TEST_PREFIX "retunIntBoolResultOrSpecError") {
  {
    auto res = retunIntBoolResultOrSpecError(true, false);
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto res = retunIntBoolResultOrSpecError(false, false);
    REQUIRE(res.is_error() == false);
    REQUIRE(res.get<0>() == 3);
    REQUIRE(res.get<1>() == true);
  }
  {
    auto res = retunIntBoolResultOrSpecError(true, true);
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
  {
    auto res = retunIntBoolResultOrSpecError(false, true);
    REQUIRE(res.is_error() == true);
    REQUIRE(res.get_error().get() == TErrCode::Err1);
  }
}

TEST_CASE(RENE_TEST_PREFIX "polymorphism") {
  struct FooBase {};
  struct FooDerived : public FooBase {};

  {
    auto func = []() -> result<std::unique_ptr<FooBase>> {
      return std::make_unique<FooDerived>();
    };
    auto res = func();
    REQUIRE_FALSE(res.is_error());
    res = std::make_unique<FooDerived>();
    REQUIRE_FALSE(res.is_error());
  }
  {
    auto func =
        []() -> result<std::unique_ptr<FooBase>, std::unique_ptr<FooBase>> {
      return {std::make_unique<FooDerived>(), std::make_unique<FooDerived>()};
    };
    auto res = func();
    REQUIRE_FALSE(res.is_error());
    res = std::make_tuple<std::unique_ptr<FooDerived>,
                          std::unique_ptr<FooDerived>>(
        std::make_unique<FooDerived>(), std::make_unique<FooDerived>());
    REQUIRE_FALSE(res.is_error());
  }
}

}  // namespace
}  // namespace test
}  // namespace rene