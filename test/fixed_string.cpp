#include <kayak/fixed_string.hpp>

#include <catch2/catch_test_macros.hpp>

#include <string_view>

using namespace std::string_view_literals;

TEST_CASE("fixed_string construction", "[fixed_string]")
{
  static constexpr auto fs = kayak::fixed_string{"test"};

  STATIC_REQUIRE(fs.data[4] == '\0');
  STATIC_REQUIRE(fs.data == "test"sv);
}

TEST_CASE("fixed_string concatenation", "[fixed_string]")
{
  static constexpr auto fs1 = kayak::fixed_string{"test"};
  static constexpr auto fs2 = kayak::fixed_string{"test2"};
  static constexpr auto fs3 = fs1 + fs2;

  STATIC_REQUIRE(fs3.data[9] == '\0');
  STATIC_REQUIRE(fs3.data == "testtest2"sv);
}
