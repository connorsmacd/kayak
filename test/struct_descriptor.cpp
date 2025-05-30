#include <kayak/struct_descriptor.hpp>

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

int main()
{
  static_assert(kayak::described_struct<test_struct>);
}
