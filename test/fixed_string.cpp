#include <kayak/fixed_string.hpp>

#include <string_view>

using namespace std::string_view_literals;

// TODO: Improve unit testing framework

int main()
{
  static constexpr auto fs = kayak::fixed_string{"test"};

  static_assert("test"sv == fs.data);
}
