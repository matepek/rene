#include "rene/error.hpp"
#include "rene/general_result_with_variant.hpp"

namespace rene {

template <typename... ResultTs>
using result = general_result_with_variant<error, ResultTs...>;

template <typename SpecialErrorT, typename... ResultTs>
using spec_result =
    general_result_with_variant<spec_error<SpecialErrorT>, ResultTs...>;

}  // namespace rene

#define RENE_TEST_PREFIX "(variant) "

#include "rene.test.result.hpp"
#include "rene.test.result_macro.hpp"

TEST_CASE("optional vs variant") {
  using namespace std;

  CHECK(8 == sizeof(variant<monostate>));

  CHECK(4 == sizeof(int32_t));
  CHECK(4 == sizeof(tuple<int32_t>));
  CHECK(8 == sizeof(optional<int32_t>));
  CHECK(8 == sizeof(variant<int32_t>));
  CHECK(8 == sizeof(variant<monostate, int32_t>));

  CHECK(8 == sizeof(uint64_t));
  CHECK(8 == sizeof(tuple<uint64_t>));
  CHECK(16 == sizeof(optional<uint64_t>));
  CHECK(16 == sizeof(variant<uint64_t>));
  CHECK(16 == sizeof(variant<monostate, uint64_t>));

  CHECK(12 == sizeof(int32_t) + sizeof(uint64_t));
  CHECK(16 == sizeof(tuple<int32_t, uint64_t>));
  CHECK(16 == sizeof(tuple<uint64_t, int32_t>));
  CHECK(24 == sizeof(optional<tuple<int32_t, uint64_t>>));
  CHECK(24 == sizeof(variant<tuple<int32_t, uint64_t>>));

  CHECK(24 == sizeof(unique_ptr<uint64_t>) + sizeof(optional<uint64_t>));
  CHECK(16 == sizeof(variant<unique_ptr<uint64_t>, uint64_t>));

  CHECK(24 == sizeof(unique_ptr<uint64_t>) + sizeof(optional<tuple<uint64_t>>));
  CHECK(16 == sizeof(variant<unique_ptr<uint64_t>, std::tuple<uint64_t>>));
}
