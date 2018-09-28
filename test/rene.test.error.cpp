#include "catch2/catch.hpp"

#include "rene/error.hpp"

namespace rene {
namespace test {
enum class ErrCode { Error1, Error2 };

static_assert(std::is_nothrow_constructible<error,
                                            error::MessageT&&,
                                            error::FileT&&,
                                            uint32_t>::value);
static_assert(!std::is_nothrow_copy_constructible<error>::value);
static_assert(std::is_nothrow_move_constructible<error>::value);
static_assert(!std::is_nothrow_copy_assignable<error>::value);
static_assert(std::is_nothrow_move_assignable<error>::value);
static_assert(std::is_nothrow_destructible<error>::value);

static_assert(std::is_nothrow_constructible<spec_error<ErrCode>,
                                            ErrCode,
                                            spec_error<ErrCode>::FileT&&,
                                            uint32_t>::value);
static_assert(!std::is_nothrow_copy_constructible<spec_error<ErrCode>>::value);
static_assert(std::is_nothrow_move_constructible<spec_error<ErrCode>>::value);
static_assert(!std::is_nothrow_copy_assignable<spec_error<ErrCode>>::value);
static_assert(std::is_nothrow_move_assignable<spec_error<ErrCode>>::value);
static_assert(std::is_nothrow_destructible<spec_error<ErrCode>>::value);

TEST_CASE("error") {
  error e("Coyote has catched the Roadrunner!", __FILE__, __LINE__);

  auto ee = error("Using with macro", __FILE__, __LINE__);

  auto eee(ee);
}

TEST_CASE("spec_error<...>") {
  spec_error<ErrCode> e(ErrCode::Error1, __FILE__, __LINE__);

  REQUIRE(e.is(ErrCode::Error1));
  REQUIRE(e.get() == ErrCode::Error1);

  REQUIRE(!e.is(ErrCode::Error2));
  REQUIRE(e.get() != ErrCode::Error2);

  auto ee = spec_error<ErrCode>(ErrCode::Error1, __FILE__, __LINE__);

  REQUIRE(ee.is(ErrCode::Error1));
  REQUIRE(ee.get() == ErrCode::Error1);

  REQUIRE(!ee.is(ErrCode::Error2));
  REQUIRE(ee.get() != ErrCode::Error2);

  using FooError = spec_error<ErrCode>;

  FooError eee = spec_error<ErrCode>(ErrCode::Error1, __FILE__, __LINE__);

  REQUIRE(eee.is(ErrCode::Error1));
  REQUIRE(eee.get() == ErrCode::Error1);

  REQUIRE(!eee.is(ErrCode::Error2));
  REQUIRE(eee.get() != ErrCode::Error2);

  auto eeee(eee);
  FooError eeeee(std::move(eee));
}

TEST_CASE("RENE_ERROR") {
  auto ee = RENE_ERROR("Using with macro");
}

TEST_CASE("RENE_SPEC_ERROR") {
  auto ee = RENE_SPEC_ERROR(ErrCode::Error1);

  REQUIRE(ee.is(ErrCode::Error1));
  REQUIRE(ee.get() == ErrCode::Error1);

  REQUIRE(!ee.is(ErrCode::Error2));
  REQUIRE(ee.get() != ErrCode::Error2);

  using FooError = spec_error<ErrCode>;

  FooError eee = RENE_SPEC_ERROR(ErrCode::Error1);

  REQUIRE(eee.is(ErrCode::Error1));
  REQUIRE(eee.get() == ErrCode::Error1);

  REQUIRE(!eee.is(ErrCode::Error2));
  REQUIRE(eee.get() != ErrCode::Error2);
}

}  // namespace test
}  // namespace rene