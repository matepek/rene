#include "catch2/catch.hpp"

#include <iostream>

#include "rene/result.hpp"

namespace rene {
namespace test {
TEST_CASE("result<>") {
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

TEST_CASE("retunEmptyResultOrError") {
  REQUIRE(retunEmptyResultOrError(true, false).is_error() == false);
  REQUIRE(retunEmptyResultOrError(false, false).is_error() == false);
  REQUIRE(retunEmptyResultOrError(true, true).is_error() == true);
  REQUIRE(retunEmptyResultOrError(false, true).is_error() == true);
}

TEST_CASE("result<int>") {
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

TEST_CASE("retunIntResultOrError") {
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

TEST_CASE("result<int,bool>") {
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

TEST_CASE("retunIntBoolResultOrError") {
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

enum class TErrCode { Err1, Err2 };
using TError = spec_error<TErrCode>;

TEST_CASE("spec_result<TErrCode>") {
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

TEST_CASE("retunEmptyResultOrSpecError") {
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

TEST_CASE("spec_result<TErrCode, int>") {
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

TEST_CASE("retunIntResultOrSpecError") {
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

TEST_CASE("spec_result<TErrCode,int,bool>") {
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

TEST_CASE("retunIntBoolResultOrSpecError") {
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
}  // namespace test
}  // namespace rene