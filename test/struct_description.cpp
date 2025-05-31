#include <kayak/struct_description.hpp>

#include <iostream>

#include "assert.hpp"

// TODO: Improve unit testing framework

using namespace std::string_view_literals;

struct test_struct {
  int a{};
  double b{};
  char c{};
};

struct nested_test_struct {
  int x{};
  test_struct y{};
  bool z{};
};

namespace kayak
{
template <>
struct struct_description<test_struct> {
  using members = member_list<{"a", &test_struct::a},
                              {"b", &test_struct::b},
                              {"c", &test_struct::c}>;
};

template <>
struct struct_description<nested_test_struct> {
  using members = member_list<{"x", &nested_test_struct::x},
                              {"y", &nested_test_struct::y},
                              {"z", &nested_test_struct::z}>;
};
} // namespace kayak

template <typename... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

int main()
{
  static_assert(kayak::described_struct<test_struct>);

  auto const test_struct_0 = test_struct{.a = 1, .b = 2.0, .c = 'c'};

  kayak::for_each_member(
    overload{[](std::string_view const name, int const a) {
               assert(name == "a"sv);
               assert(a == 1);
             },
             [](std::string_view const name, double const b) {
               assert(name == "b"sv);
               assert(b == 2.0);
             },
             [](std::string_view const name, char const c) {
               assert(name == "c"sv);
               assert(c == 'c');
             }},
    test_struct_0);

  assert(std::format("{}", test_struct_0) == "{a: 1, b: 2, c: c}"sv);

  auto const nested_test_struct_0 = nested_test_struct{
    .x = 42, .y = test_struct{.a = 1, .b = 2.0, .c = 'c'}, .z = true};

  assert(std::format("{}", nested_test_struct_0)
         == "{x: 42, y: {a: 1, b: 2, c: c}, z: true}"sv);
}
