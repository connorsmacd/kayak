#include <kayak/struct_descriptor.hpp>

#include "assert.hpp"

// TODO: Improve unit testing framework

struct test_struct {
  int a{};
  double b{};
  char c{};
};

namespace kayak
{
template <>
struct struct_descriptor<test_struct> {
  using members = member_list<{"a", &test_struct::a},
                              {"b", &test_struct::b},
                              {"c", &test_struct::c}>;
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

  kayak::visit_members(overload{[](int const a) { assert(a == 1); },
                                [](double const b) { assert(b == 2.0); },
                                [](char const c) { assert(c == 'c'); }},
                       test_struct_0);
}
