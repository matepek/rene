#include "catch2/catch.hpp"

#include "rene/error.hpp"

namespace rene {
namespace test {
enum class FooErrorCode { Error1, Error2 };

TEST_CASE("error") {
  error e("Coyote has catched the Roadrunner!", __FILE__, __LINE__);

  auto ee = ERROR("Using with macro");

  auto eee(ee);

  error eeee(std::move(ee), __FILE__, __LINE__);
}

TEST_CASE("spec_error<...>") {
  spec_error<FooErrorCode> e(FooErrorCode::Error1, __FILE__, __LINE__);

  REQUIRE(e.is(FooErrorCode::Error1));
  REQUIRE(e.get() == FooErrorCode::Error1);

  REQUIRE(!e.is(FooErrorCode::Error2));
  REQUIRE(e.get() != FooErrorCode::Error2);

  auto ee = SPEC_ERROR(FooErrorCode::Error1);

  REQUIRE(ee.is(FooErrorCode::Error1));
  REQUIRE(ee.get() == FooErrorCode::Error1);

  REQUIRE(!ee.is(FooErrorCode::Error2));
  REQUIRE(ee.get() != FooErrorCode::Error2);

  using FooError = spec_error<FooErrorCode>;

  FooError eee = SPEC_ERROR(FooErrorCode::Error1);

  REQUIRE(eee.is(FooErrorCode::Error1));
  REQUIRE(eee.get() == FooErrorCode::Error1);

  REQUIRE(!eee.is(FooErrorCode::Error2));
  REQUIRE(eee.get() != FooErrorCode::Error2);

  auto eeee(eee);
  FooError eeeee(std::move(eee));
}

}  // namespace test
}  // namespace rene