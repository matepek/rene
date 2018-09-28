#include "rene/error.hpp"
#include "rene/general_result_with_optional.hpp"

namespace rene {

template <typename... ResultTs>
using result = general_result_with_optional<error, ResultTs...>;

template <typename SpecialErrorT, typename... ResultTs>
using spec_result =
    general_result_with_optional<spec_error<SpecialErrorT>, ResultTs...>;

}  // namespace rene

#define RENE_TEST_PREFIX "(optional) "

#include "rene.test.result.hpp"
#include "rene.test.result_macro.hpp"