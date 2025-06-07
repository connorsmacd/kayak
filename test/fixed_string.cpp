#include <kayak/fixed_string.hpp>

#include <string_view>

using namespace std::string_view_literals;

// TODO: Improve unit testing framework

int main()
{
  static constexpr auto fs1 = kayak::fixed_string{"test"};

  static_assert("test"sv == fs1.data);

  static constexpr auto fs2 = kayak::fixed_string{"test2"};
  static constexpr auto fs3 = fs1 + fs2;

  static_assert("testtest2"sv == fs3.data);
}
