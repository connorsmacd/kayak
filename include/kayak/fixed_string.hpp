#ifndef KAYAK_FIXED_STRING_HPP
#define KAYAK_FIXED_STRING_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string_view>

namespace kayak
{
template <std::size_t L>
struct fixed_string {
  char data[L + 1]{};

  constexpr fixed_string() noexcept = default;

  constexpr fixed_string(char const (&c_str)[L + 1]) noexcept
  {
    std::ranges::copy(c_str, data);
  }

  using const_iterator = char const*;
  using iterator = char*;

  constexpr auto cbegin() const noexcept -> const_iterator { return data; }
  constexpr auto begin() const noexcept -> const_iterator { return cbegin(); }
  constexpr auto begin() noexcept -> iterator { return data; }

  constexpr auto cend() const noexcept -> const_iterator
  {
    return std::next(data, L);
  }
  constexpr auto end() const noexcept -> const_iterator { return cend(); }
  constexpr auto end() noexcept -> iterator { return std::next(data, L); }
};

template <std::size_t N>
fixed_string(char const (&c_str)[N]) -> fixed_string<N - 1>;

template <typename T>
struct is_fixed_string : std::false_type {};

template <std::size_t L>
struct is_fixed_string<fixed_string<L>> : std::true_type {};

inline namespace literals
{
inline namespace fixed_string_literals
{
template <fixed_string F>
constexpr auto operator""_fs() noexcept
{
  return F;
}
} // namespace fixed_string_literals
} // namespace literals

template <std::size_t L>
constexpr auto to_string_view(fixed_string<L> const& fs)
{
  return std::string_view(fs.data, L);
}

template <std::size_t L1, std::size_t L2>
constexpr auto operator==(fixed_string<L1> const& lhs,
                          fixed_string<L2> const& rhs) noexcept -> bool
{
  if constexpr (L1 != L2)
    return false;
  else
    return to_string_view(lhs) == to_string_view(rhs);
}

template <std::size_t L1, std::size_t L2>
constexpr auto operator<=>(fixed_string<L1> const& lhs,
                           fixed_string<L2> const& rhs) noexcept
{
  return to_string_view(lhs) <=> to_string_view(rhs);
}

template <std::size_t L1, std::size_t L2>
constexpr auto operator+(fixed_string<L1> const& lhs,
                         fixed_string<L2> const& rhs) noexcept
  -> fixed_string<L1 + L2>
{
  auto result = fixed_string<L1 + L2>{};

  std::ranges::copy(lhs, result.begin());
  std::ranges::copy(rhs, std::next(result.begin(), L1));

  return result;
}

template <std::size_t L, std::size_t N>
constexpr auto operator+(fixed_string<L> const& lhs,
                         char const (&c_str)[N]) noexcept
  -> fixed_string<L + N - 1>
{
  return lhs + fixed_string{c_str};
}

template <std::size_t N, std::size_t L>
constexpr auto operator+(char const (&c_str)[N],
                         fixed_string<L> const& rhs) noexcept
  -> fixed_string<N + L - 1>
{
  return fixed_string{c_str} + rhs;
}

namespace detail
{
template <std::size_t Digit>
  requires(Digit < 10)
constexpr auto digit_to_fixed_string() -> fixed_string<1>
{
  auto result = fixed_string<1>{};
  result.data[0] = '0' + static_cast<char>(Digit);
  return result;
}

template <std::size_t Number>
constexpr auto to_fixed_string()
{
  if constexpr (Number < 10)
    return detail::digit_to_fixed_string<Number>();
  else
    return to_fixed_string<Number / 10>()
           + detail::digit_to_fixed_string<Number % 10>();
}
} // namespace detail

template <std::size_t Number>
static constexpr auto as_fixed_string = detail::to_fixed_string<Number>();

namespace detail
{
template <std::size_t DelimiterL, std::size_t LastL>
constexpr auto join_impl(fixed_string<DelimiterL> const&,
                         fixed_string<LastL> const& last_str)
{
  return last_str;
}

template <std::size_t DelimiterL, std::size_t LeftL, std::size_t... RightLs>
constexpr auto join_impl(fixed_string<DelimiterL> const& delimiter,
                         fixed_string<LeftL> const& left_strs,
                         fixed_string<RightLs> const&... right_strs)
{
  return left_strs + delimiter + join_impl(delimiter, right_strs...);
}
} // namespace detail

template <std::size_t DelimiterL, std::size_t... Ls>
constexpr auto join(fixed_string<DelimiterL> const& delimiter,
                    fixed_string<Ls> const&... strs)
  -> fixed_string<(... + Ls) + DelimiterL * (sizeof...(Ls) - 1)>
{
  return detail::join_impl(delimiter, strs...);
}

// TODO: Figure out why args cannot be deduced from C-style string.
template <std::size_t DelimiterN, std::size_t... Ls>
constexpr auto join(char const (&delimiter_c_str)[DelimiterN],
                    fixed_string<Ls> const&... strs)
{
  return join(fixed_string{delimiter_c_str}, strs...);
}
} // namespace kayak

#endif
