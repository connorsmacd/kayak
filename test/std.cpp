#include <kayak/std/optional.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace kayak::fixed_string_literals;

TEST_CASE("std::optional type_description", "[std::optional]")
{
  STATIC_REQUIRE(kayak::name_of<std::optional<int>> == "std::optional<int>"_fs);
}
