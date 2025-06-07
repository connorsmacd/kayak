#include <kayak/enum_description.hpp>

#include "assert.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

enum class contiguous_test_enum {
  a = -5,
  b,
  c,
  d,
};

template <>
struct kayak::enum_description<contiguous_test_enum> {
  static constexpr auto string_view_enumerator_pairs = [] {
    using enum contiguous_test_enum;
    return std::to_array<std::pair<std::string_view, contiguous_test_enum>>(
      {{"d", d}, {"a", a}, {"c", c}, {"b", b}});
  }();
};

enum class non_contiguous_test_enum {
  a,
  b = 2,
  c = 4,
  d = 6,
};

template <>
struct kayak::enum_description<non_contiguous_test_enum> {
  static constexpr auto string_view_enumerator_pairs = [] {
    using enum non_contiguous_test_enum;
    return std::to_array<std::pair<std::string_view, non_contiguous_test_enum>>(
      {{"a", a}, {"b", b}, {"c", c}, {"d", d}});
  }();
};

int main()
{
  static_assert(kayak::described_enum<contiguous_test_enum>);
  static_assert(kayak::continguous_described_enum<contiguous_test_enum>);

  static_assert(kayak::to_string_view(contiguous_test_enum::a) == "a"sv);
  static_assert(kayak::to_string_view(contiguous_test_enum::b) == "b"sv);
  static_assert(kayak::to_string_view(contiguous_test_enum::c) == "c"sv);
  static_assert(kayak::to_string_view(contiguous_test_enum::d) == "d"sv);

  assert(kayak::to_string(contiguous_test_enum::a) == "a"s);
  assert(kayak::to_string(contiguous_test_enum::b) == "b"s);

  assert(std::format("{}", contiguous_test_enum::a) == "a"s);
  assert(std::format("{}", contiguous_test_enum::b) == "b"s);

  static_assert(kayak::from_string<contiguous_test_enum>("a"sv)
                == contiguous_test_enum::a);
  static_assert(kayak::from_string<contiguous_test_enum>("b"sv)
                == contiguous_test_enum::b);
  static_assert(kayak::from_string<contiguous_test_enum>("c"sv)
                == contiguous_test_enum::c);
  static_assert(kayak::from_string<contiguous_test_enum>("d"sv)
                == contiguous_test_enum::d);

  static_assert(kayak::described_enum<non_contiguous_test_enum>);
  static_assert(!kayak::continguous_described_enum<non_contiguous_test_enum>);

  static_assert(kayak::to_string_view(non_contiguous_test_enum::a) == "a"sv);
  static_assert(kayak::to_string_view(non_contiguous_test_enum::b) == "b"sv);
  static_assert(kayak::to_string_view(non_contiguous_test_enum::c) == "c"sv);
  static_assert(kayak::to_string_view(non_contiguous_test_enum::d) == "d"sv);

  assert(kayak::to_string(non_contiguous_test_enum::a) == "a"s);
}
