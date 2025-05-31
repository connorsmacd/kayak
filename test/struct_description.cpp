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

namespace test_bases
{
struct a {
  int a_member{};
};

struct b : a {
  double b_member{};
};

struct c {
  char c_member{};
};

struct d : b, c {
  bool d_member{};
};
} // namespace test_bases

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

template <>
struct struct_description<test_bases::a> {
  using members = member_list<{"a_member", &test_bases::a::a_member}>;
};
template <>
struct struct_description<test_bases::b> {
  using bases = type_list<test_bases::a>;
  using members = member_list<{"b_member", &test_bases::b::b_member}>;
};
template <>
struct struct_description<test_bases::c> {
  using members = member_list<{"c_member", &test_bases::c::c_member}>;
};
template <>
struct struct_description<test_bases::d> {
  using bases = type_list<test_bases::b, test_bases::c>;
  using members = member_list<{"d_member", &test_bases::d::d_member}>;
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

  auto const test_bases_d_0 = [] {
    test_bases::d d;
    d.a_member = 1;
    d.b_member = 2.0;
    d.c_member = 'c';
    d.d_member = true;
    return d;
  }();

  kayak::for_each_base(
    overload{
      [](test_bases::a const& a) { assert(a.a_member == 1); },
      [](test_bases::b const& b) { assert(b.b_member == 2.0); },
      [](test_bases::c const& c) { assert(c.c_member == 'c'); },
    },
    test_bases_d_0);

  kayak::for_each_member_recurse_bases(
    overload{[](std::string_view const name, int const a) {
               assert(name == "a_member"sv);
               assert(a == 1);
             },
             [](std::string_view const name, double const b) {
               assert(name == "b_member"sv);
               assert(b == 2.0);
             },
             [](std::string_view const name, char const c) {
               assert(name == "c_member"sv);
               assert(c == 'c');
             },
             [](std::string_view const name, bool const d) {
               assert(name == "d_member"sv);
               assert(d == true);
             }},
    test_bases_d_0);
}
