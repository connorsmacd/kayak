#include <kayak/std/allocator.hpp>
#include <kayak/std/array.hpp>
#include <kayak/std/optional.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace kayak::fixed_string_literals;

TEST_CASE("std::allocator type_description", "[std::allocator]")
{
  STATIC_REQUIRE(kayak::name_of<std::allocator<int>>
                 == "std::allocator<int>"_fs);
}

TEST_CASE("std::array type_description", "[std::array]")
{
  STATIC_REQUIRE(kayak::name_of<std::array<int, 42>>
                 == "std::array<int, 42>"_fs);
}

TEST_CASE("std::optional type_description", "[std::optional]")
{
  STATIC_REQUIRE(kayak::name_of<std::optional<int>> == "std::optional<int>"_fs);
}
