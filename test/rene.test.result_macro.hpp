
#include <iostream>

#include "rene/general_result_macro.hpp"

namespace rene {
namespace test {
namespace {

struct What {
  enum Type {
    Constructed,
    CopyConstructed,
    MoveConstructed,
    CopyAssigned,
    MoveAssigned,
    CalledAsLeftVal,
    CalledAsRightVal
  };

  std::vector<Type> action;

  What() : action({Constructed}) {}
  What(const What& other) : action(other.action) {
    action.push_back(CopyConstructed);
  }
  What(What&& other) : action(other.action) {
    action.push_back(MoveConstructed);
  }

  What& operator=(const What& other) {
    action = other.action;
    action.push_back(CopyAssigned);
    return *this;
  }
  What& operator=(What&& other) {
    action = other.action;
    action.push_back(MoveAssigned);
    return *this;
  }

  void call() & { action.push_back(CalledAsLeftVal); }
  void call() && { action.push_back(CalledAsRightVal); }
};

// For nicer error message
std::ostream& operator<<(std::ostream& os, const What::Type& t) {
  switch (t) {
    case What::Constructed:
      os << "Constructed";
      return os;
    case What::CopyConstructed:
      os << "CopyConstructed";
      return os;
    case What::MoveConstructed:
      os << "MoveConstructed";
      return os;
    case What::CopyAssigned:
      os << "CopyAssigned";
      return os;
    case What::MoveAssigned:
      os << "MoveAssigned";
      return os;
    case What::CalledAsLeftVal:
      os << "CalledAsLeftVal";
      return os;
    case What::CalledAsRightVal:
      os << "CalledAsRightVal";
      return os;
  }
}

using Equals = Catch::Vector::EqualsMatcher<What::Type>;

template <typename... Args>
result<Args...> returnResultOrError(bool returnWithError) {
  result<Args...> res(Args{}...);
  if (returnWithError)
    res = error("returnResultOrError", __FILE__, __LINE__);
  return res;
}

enum class Err { Err1, Err2 };

template <typename... Args>
spec_result<Err, Args...> returnSpecResultOrError(bool returnWithError) {
  spec_result<Err, Args...> res(Args{}...);
  if (returnWithError)
    res = spec_error<Err>(Err::Err1, __FILE__, __LINE__);
  return res;
}

TEST_CASE(RENE_TEST_PREFIX "result<What>") {
  {
    auto func = []() -> result<What> { return What(); };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
      REQUIRE_THAT(res.get().action,
                   Equals({What::Constructed, What::MoveConstructed}));
    }
    auto func_return_func = [&]() -> result<What> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
    REQUIRE_THAT(res.get().action,
                 Equals({What::Constructed, What::MoveConstructed}));
  }
  {
    auto func = []() -> result<What> {
      return returnResultOrError<What>(false);
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
      REQUIRE_THAT(res.get().action,
                   Equals({What::Constructed, What::MoveConstructed}));
    }
    auto func_return_func = [&]() -> result<What> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
    REQUIRE_THAT(res.get().action,
                 Equals({What::Constructed, What::MoveConstructed}));
  }
}

TEST_CASE(RENE_TEST_PREFIX "spec_result<Err, What>") {
  {
    auto func = []() -> spec_result<Err, What> { return What(); };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
      REQUIRE_THAT(res.get().action,
                   Equals({What::Constructed, What::MoveConstructed}));
    }
    auto func_return_func = [&]() -> spec_result<Err, What> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
    REQUIRE_THAT(res.get().action,
                 Equals({What::Constructed, What::MoveConstructed}));
  }
  {
    auto func = []() -> spec_result<Err, What> {
      return returnSpecResultOrError<What>(false);
    };
    {
      auto res = func();
      REQUIRE(res.is_error() == false);
      REQUIRE_THAT(res.get().action,
                   Equals({What::Constructed, What::MoveConstructed}));
    }
    auto func_return_func = [&]() -> spec_result<Err, What> { return func(); };
    auto res = func_return_func();
    REQUIRE(res.is_error() == false);
    REQUIRE_THAT(res.get().action,
                 Equals({What::Constructed, What::MoveConstructed}));
  }
}

TEST_CASE(RENE_TEST_PREFIX "Macro RENE_RETURN") {
  SECTION("result<>") {
    auto func = [&](bool returnWithError) -> result<> {
      RENE_RETURN(returnResultOrError(returnWithError));
    };
    SECTION("returnWithError") { CHECK(func(true).is_error() == true); }
    SECTION("!returnWithError") { CHECK(func(false).is_error() == false); }
  }
  SECTION("spec_result<Err>") {
    auto func = [&](bool returnWithError) -> spec_result<Err> {
      RENE_RETURN(returnSpecResultOrError(returnWithError));
    };
    SECTION("returnWithError") { CHECK(func(true).is_error() == true); }
    SECTION("!returnWithError") { CHECK(func(false).is_error() == false); }
  }
  SECTION("result<What>") {
    auto func = [&](bool returnWithError) -> result<What> {
      RENE_RETURN(returnResultOrError<What>(returnWithError));
    };
    SECTION("returnWithError") { CHECK(func(true).is_error() == true); }
    SECTION("!returnWithError") {
      auto res = func(false);
      REQUIRE_FALSE(res.is_error());
      CHECK_THAT(res.get().action,
                 Equals({What::Constructed, What::MoveConstructed,
                         What::MoveConstructed}));
    }
    SECTION("2 frame") {
      auto func2 = [&]() -> result<What> { RENE_RETURN(func(false)); };
      auto res2 = func2();
      REQUIRE_FALSE(res2.is_error());
      CHECK_THAT(res2.get().action,
                 Equals({What::Constructed, What::MoveConstructed,
                         What::MoveConstructed, What::MoveConstructed}));
    }
  }
  SECTION("spec_result<Err, What>") {
    auto func = [&](bool returnWithError) -> spec_result<Err, What> {
      RENE_RETURN(returnSpecResultOrError<What>(returnWithError));
    };
    SECTION("returnWithError") { CHECK(func(true).is_error() == true); }
    SECTION("!returnWithError") {
      auto res = func(false);
      REQUIRE_FALSE(res.is_error());
      CHECK_THAT(res.get().action,
                 Equals({What::Constructed, What::MoveConstructed,
                         What::MoveConstructed}));
    }
  }
  SECTION("result<What, What>") {
    auto func = [&](bool returnWithError) -> result<What, What> {
      RENE_RETURN(returnResultOrError<What, What>(returnWithError));
    };
    SECTION("returnWithError") { CHECK(func(true).is_error() == true); }
    SECTION("!returnWithError") {
      auto res = func(false);
      REQUIRE_FALSE(res.is_error());
      CHECK_THAT(res.get<0>().action,
                 Equals({What::Constructed, What::MoveConstructed,
                         What::MoveConstructed}));
      CHECK_THAT(res.get<1>().action,
                 Equals({What::Constructed, What::MoveConstructed,
                         What::MoveConstructed}));
    }
  }
  SECTION("spec_result<Err, What, What>") {
    auto func = [&](bool returnWithError) -> spec_result<Err, What, What> {
      RENE_RETURN(returnSpecResultOrError<What, What>(returnWithError));
    };
    SECTION("returnWithError") { CHECK(func(true).is_error() == true); }
    SECTION("!returnWithError") {
      auto res = func(false);
      REQUIRE_FALSE(res.is_error());
      CHECK_THAT(res.get<0>().action,
                 Equals({What::Constructed, What::MoveConstructed,
                         What::MoveConstructed}));
      CHECK_THAT(res.get<1>().action,
                 Equals({What::Constructed, What::MoveConstructed,
                         What::MoveConstructed}));
    }
  }
}

TEST_CASE(RENE_TEST_PREFIX "Macro RENE_RETURN_IF_ERROR") {
  SECTION("result<>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> result<> {
      overstepped = false;
      RENE_RETURN_IF_ERROR(returnResultOrError(returnWithError));
      overstepped = true;
      return {};
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error() == true);
      CHECK(overstepped == false);
    }
    SECTION("!returnWithError") {
      REQUIRE_FALSE(func(false).is_error());
      REQUIRE(overstepped == true);
    }
  }
  SECTION("spec_result<Err>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> spec_result<Err> {
      overstepped = false;
      RENE_RETURN_IF_ERROR(returnSpecResultOrError(returnWithError));
      overstepped = true;
      return {};
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error() == true);
      CHECK(overstepped == false);
    }
    SECTION("!returnWithError") {
      REQUIRE_FALSE(func(false).is_error());
      REQUIRE(overstepped == true);
    }
  }
  SECTION("result<What>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> result<> {
      overstepped = false;
      RENE_RETURN_IF_ERROR(returnResultOrError<What>(returnWithError));
      overstepped = true;
      return {};
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error() == true);
      CHECK(overstepped == false);
    }
    SECTION("!returnWithError") {
      REQUIRE_FALSE(func(false).is_error());
      REQUIRE(overstepped == true);
    }
  }
  SECTION("spec_result<Err, What>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> spec_result<Err> {
      overstepped = false;
      RENE_RETURN_IF_ERROR(returnSpecResultOrError<What>(returnWithError));
      overstepped = true;
      return {};
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error() == true);
      CHECK(overstepped == false);
    }
    SECTION("!returnWithError") {
      REQUIRE_FALSE(func(false).is_error());
      REQUIRE(overstepped == true);
    }
  }
  SECTION("result<What, What>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> result<> {
      overstepped = false;
      RENE_RETURN_IF_ERROR(returnResultOrError<What, What>(returnWithError));
      overstepped = true;
      return {};
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error() == true);
      CHECK(overstepped == false);
    }
    SECTION("!returnWithError") {
      REQUIRE_FALSE(func(false).is_error());
      REQUIRE(overstepped == true);
    }
  }
  SECTION("spec_result<Err, What, What>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> spec_result<Err> {
      overstepped = false;
      RENE_RETURN_IF_ERROR(
          returnSpecResultOrError<What, What>(returnWithError));
      overstepped = true;
      return {};
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error() == true);
      CHECK(overstepped == false);
    }
    SECTION("!returnWithError") {
      REQUIRE_FALSE(func(false).is_error());
      REQUIRE(overstepped == true);
    }
  }
}

TEST_CASE(RENE_TEST_PREFIX "Macro RENE_RETURN_OR_CREATE") {
  SECTION("result<What>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> result<> {
      overstepped = false;

      RENE_RETURN_OR_CREATE(result0, []() -> result<What> { return What{}; }());
      REQUIRE_THAT(result0.action,
                   Equals({What::Constructed, What::MoveConstructed}));
      RENE_RETURN_OR_CREATE(result, returnResultOrError<What>(returnWithError));
      REQUIRE_THAT(result.action,
                   Equals({What::Constructed, What::MoveConstructed}));
      overstepped = true;
      return {};
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error());
      CHECK_FALSE(overstepped);
    }
    SECTION("!returnWithError") {
      CHECK_FALSE(func(false).is_error());
      CHECK(overstepped);
    }
  }
  SECTION("spec_result<Err, What>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> spec_result<Err> {
      overstepped = false;
      RENE_RETURN_OR_CREATE(result,
                            returnSpecResultOrError<What>(returnWithError));
      REQUIRE_THAT(result.action,
                   Equals({What::Constructed, What::MoveConstructed}));
      overstepped = true;
      return {};
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error());
      CHECK_FALSE(overstepped);
    }
    SECTION("!returnWithError") {
      CHECK_FALSE(func(false).is_error());
      CHECK(overstepped);
    }
  }
  SECTION("result<What, What>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> result<What> {
      overstepped = false;
      RENE_RETURN_OR_CREATE(result, returnResultOrError<What>(returnWithError));
      REQUIRE_THAT(result.action,
                   Equals({What::Constructed, What::MoveConstructed}));
      overstepped = true;
      return What();
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error());
      CHECK_FALSE(overstepped);
    }
    SECTION("!returnWithError") {
      CHECK_FALSE(func(false).is_error());
      CHECK(overstepped);
    }
  }
  SECTION("spec_result<Err, What, What>") {
    bool overstepped = false;
    auto func = [&](bool returnWithError) -> spec_result<Err, What> {
      overstepped = false;
      RENE_RETURN_OR_CREATE(result,
                            returnSpecResultOrError<What>(returnWithError));
      REQUIRE_THAT(result.action,
                   Equals({What::Constructed, What::MoveConstructed}));
      overstepped = true;
      return What();
    };
    SECTION("returnWithError") {
      CHECK(func(true).is_error());
      CHECK_FALSE(overstepped);
    }
    SECTION("!returnWithError") {
      CHECK_FALSE(func(false).is_error());
      CHECK(overstepped);
    }
  }
}

TEST_CASE(RENE_TEST_PREFIX "return<&>") {
  SECTION("const &") {
    What w;
    auto func = [&]() -> result<const What&> { return w; };
    auto res = func();
    REQUIRE_FALSE(res.is_error());
    REQUIRE_THAT(res.get().action, Equals({What::Constructed}));
    REQUIRE_THAT(res->action, Equals({What::Constructed}));
  }
  SECTION("&") {
    What w;
    auto func = [&]() -> result<What&> { return w; };
    auto res = func();
    REQUIRE_FALSE(res.is_error());
    REQUIRE_THAT(res.get().action, Equals({What::Constructed}));
    REQUIRE_THAT(res->action, Equals({What::Constructed}));
  }
  SECTION("const &, const&") {
    What w1, w2;
    auto func = [&]() -> result<const What&, const What&> { return {w1, w2}; };
    auto res = func();
    REQUIRE_FALSE(res.is_error());
    REQUIRE_THAT(res.get<0>().action, Equals({What::Constructed}));
    REQUIRE_THAT(res.get<1>().action, Equals({What::Constructed}));
  }
  SECTION("const &, const&") {
    What w1, w2;
    auto func = [&]() -> result<What&, What&> { return {w1, w2}; };
    auto res = func();
    REQUIRE_FALSE(res.is_error());
    REQUIRE_THAT(res.get<0>().action, Equals({What::Constructed}));
    REQUIRE_THAT(res.get<1>().action, Equals({What::Constructed}));
  }
}

#if 0
TEST_CASE(RENE_TEST_PREFIX "HANDLE_IF_ERROR") {
  SECTION("result<>") {
    auto func = [&](bool returnWithError) -> result<> {
      return returnResultOrError(returnWithError);
    };
    SECTION("is_error") {
      bool isHandled = false;
      HANDLE_IF_ERROR(error, func(true)) { isHandled = true; }
      CHECK(isHandled);
    }
    SECTION("!is_error") {
      bool isHandled = false;
      HANDLE_IF_ERROR(error, func(false)) { isHandled = true; }
      CHECK_FALSE(isHandled);
    }
  }
  SECTION("spec_result<Err>") {
    auto func = [&](bool returnWithError) -> spec_result<Err> {
      return returnSpecResultOrError(returnWithError);
    };
    SECTION("is_error") {
      bool isHandled = false;
      HANDLE_IF_ERROR(error, func(true)) { isHandled = true; }
      CHECK(isHandled);
    }
    SECTION("!is_error") {
      bool isHandled = false;
      HANDLE_IF_ERROR(error, func(false)) { isHandled = true; }
      CHECK_FALSE(isHandled);
    }
  }
}
#endif

}  // namespace
}  // namespace test
}  // namespace rene