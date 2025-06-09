#include <kayak/fixed_string.hpp>

#include <catch2/catch_test_macros.hpp>

#include <string_view>

using namespace kayak::literals;
using namespace std::string_view_literals;

TEST_CASE("fixed_string construction", "[fixed_string]")
{
  static constexpr auto fs = kayak::fixed_string{"test"};

  STATIC_REQUIRE(fs.data[4] == '\0');
  STATIC_REQUIRE(fs.data == "test"sv);
}

TEST_CASE("fixed_string literal", "[fixed_string]")
{
  static constexpr auto fs = "test"_fs;

  STATIC_REQUIRE(fs.data[4] == '\0');
  STATIC_REQUIRE(fs.data == "test"sv);
}

TEST_CASE("fixed_string iterators", "[fixed_string]")
{
  static constexpr auto fs = "test"_fs;

  STATIC_REQUIRE(std::ranges::equal(fs, "test"sv));
}

TEST_CASE("fixed_string std::string_view conversion", "[fixed_string]")
{
  static constexpr auto fs = "test"_fs;

  STATIC_REQUIRE(to_string_view(fs) == "test"sv);
}

TEST_CASE("fixed_string comparison", "[fixed_string]")
{
  static constexpr auto fs1 = "test"_fs;
  static constexpr auto fs2 = "test"_fs;
  static constexpr auto fs3 = "test2"_fs;

  STATIC_REQUIRE(fs1 == fs2);
  STATIC_REQUIRE(fs1 != fs3);

  STATIC_REQUIRE(fs1 <= fs2);
  STATIC_REQUIRE(fs1 >= fs2);
  STATIC_REQUIRE(fs1 < fs3);
  STATIC_REQUIRE(fs1 <= fs3);
  STATIC_REQUIRE(fs3 > fs1);
  STATIC_REQUIRE(fs3 >= fs1);
}

TEST_CASE("fixed_string concatenation", "[fixed_string]")
{
  static constexpr auto fs1 = "test"_fs;
  static constexpr auto fs2 = "test2"_fs;
  static constexpr auto fs3 = fs1 + fs2;

  STATIC_REQUIRE(fs3.data[9] == '\0');
  STATIC_REQUIRE(fs3 == "testtest2"_fs);

  static constexpr auto fs4 = fs1 + "c_str";
  STATIC_REQUIRE(fs4.data[9] == '\0');
  STATIC_REQUIRE(fs4 == "testc_str"_fs);

  static constexpr auto fs5 = "c_str"_fs + fs1;
  STATIC_REQUIRE(fs5.data[9] == '\0');
  STATIC_REQUIRE(fs5 == "c_strtest"_fs);
}

TEST_CASE("fixed_string from number", "[fixed_string]")
{
  STATIC_REQUIRE(kayak::as_fixed_string<0> == "0"_fs);
  STATIC_REQUIRE(kayak::as_fixed_string<1> == "1"_fs);
  STATIC_REQUIRE(kayak::as_fixed_string<10> == "10"_fs);
  STATIC_REQUIRE(kayak::as_fixed_string<100> == "100"_fs);
  STATIC_REQUIRE(kayak::as_fixed_string<1234> == "1234"_fs);
}
