#include <kayak/struct_update.hpp>

#include <catch2/catch_test_macros.hpp>

struct test_struct {
  int a{};
  bool b{};
  char c{};

  constexpr auto operator<=>(test_struct const&) const noexcept = default;
};

namespace kayak
{
template <>
struct type_description<test_struct> :
  type_description_impl<"test_struct", test_struct> {};

template <>
struct struct_description<test_struct> {
  using members = member_list<{"a", &test_struct::a},
                              {"b", &test_struct::b},
                              {"c", &test_struct::c}>;
};
}

TEST_CASE("struct_update basic usage", "[struct_update]")
{
    static constexpr auto s0 = test_struct{ .a = 1, .b = false, .c = 'c' };

    static constexpr auto s1 = kayak::struct_update(s0).with<"b">(true).with<"c">('C')();

    STATIC_REQUIRE(s1 == test_struct{ .a = 1, .b = true, .c = 'C' });
}
