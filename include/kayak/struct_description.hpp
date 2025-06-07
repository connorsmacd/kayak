#ifndef KAYAK_STRUCT_DESCRIPTION_HPP
#define KAYAK_STRUCT_DESCRIPTION_HPP

#include <kayak/fixed_string.hpp>
#include <kayak/type_list.hpp>

#include <format>
#include <tuple>
#include <type_traits>

namespace kayak
{
template <typename T>
concept member_ptr = std::is_member_pointer_v<T>;

template <std::size_t L, member_ptr M>
struct member_description {
  fixed_string<L> name;
  M member;
};

template <std::size_t N, member_ptr M>
member_description(char const (&name)[N], M member)
  -> member_description<N - 1, M>;

template <member_description... Ms>
struct member_list {
  static constexpr auto size = sizeof...(Ms);
  static constexpr auto tuple = std::tuple{Ms...};
};

template <typename T>
struct struct_description;

template <typename T>
struct is_member_list : std::false_type {};

template <member_description... Ms>
struct is_member_list<member_list<Ms...>> : std::true_type {};

template <typename T>
concept described_struct = is_member_list<
  typename struct_description<std::remove_cv_t<T>>::members>::value;

namespace detail
{
template <typename Func, described_struct T>
constexpr void for_each_member_impl(Func& func, T& s)
{
  std::apply(
    [&](auto const&... descriptions) {
      (func(to_string_view(descriptions.name),
            std::invoke(descriptions.member, s)),
       ...);
    },
    struct_description<std::remove_cv_t<T>>::members::tuple);
}
} // namespace detail

template <typename Func, described_struct T>
constexpr auto for_each_member(Func func, T& s) -> Func
{
  detail::for_each_member_impl(func, s);
  return func;
}

template <typename T>
concept described_struct_with_bases
  = described_struct<T>
    && is_type_list<
      typename struct_description<std::remove_cv_t<T>>::bases>::value;

namespace detail
{
template <typename Func, described_struct T>
constexpr void for_each_base_impl(Func&, T&)
{
}

template <typename From, typename To>
using propagate_const_ref_t
  = std::conditional_t<std::is_const_v<From>, To const&, To&>;

template <typename Func, described_struct_with_bases T>
constexpr void for_each_base_impl(Func& func, T& s)
{
  using bases = typename struct_description<std::remove_cv_t<T>>::bases;

  std::apply(
    [&](auto const&... base_descriptions) {
      (func(static_cast<propagate_const_ref_t<
              T,
              typename std::remove_cvref_t<decltype(base_descriptions)>::type>>(
         s)),
       ...);
    },
    bases::descriptions);
}
} // namespace detail

template <typename Func, described_struct T>
constexpr auto for_each_base(Func func, T& s) -> Func
{
  detail::for_each_base_impl(func, s);
  return func;
}

namespace detail
{
template <typename Func, described_struct T>
constexpr void for_each_base_recurse_impl(Func&, T&)
{
}

template <typename Func, described_struct_with_bases T>
constexpr void for_each_base_recurse_impl(Func& func, T& s)
{
  for_each_base(
    [&](auto& base) {
      for_each_base_recurse_impl(func, base);
      std::invoke(func, base);
    },
    s);
}
} // namespace detail

template <typename Func, described_struct T>
constexpr auto for_each_base_recurse(Func func, T& s) -> Func
{
  detail::for_each_base_recurse_impl(func, s);
  return func;
}

namespace detail
{
template <typename Func, described_struct T>
constexpr void for_each_member_recurse_bases_impl(Func& func, T& s)
{
  detail::for_each_member_impl(func, s);
}

template <typename Func, described_struct_with_bases T>
constexpr void for_each_member_recurse_bases_impl(Func& func, T& s)
{
  for_each_base(
    [&](auto& base) { for_each_member_recurse_bases_impl(func, base); }, s);
  detail::for_each_member_impl(func, s);
}
} // namespace detail

template <typename Func, described_struct T>
constexpr auto for_each_member_recurse_bases(Func func, T& s) -> Func
{
  detail::for_each_member_recurse_bases_impl(func, s);
  return func;
}
} // namespace kayak

template <kayak::described_struct T>
struct std::formatter<T, char> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx)
  {
    // TODO: Consider per-member format specifiers
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(T const& s, FormatContext& ctx) const
  {
    auto out = ctx.out();

    std::format_to(out, "{{");

    kayak::for_each_member(
      [&, i = std::size_t{0}](std::string_view const name,
                              auto const& value) mutable {
        out = std::format_to(out, "{}: {}", name, value);

        if (++i < kayak::struct_description<T>::members::size)
          out = std::format_to(out, ", ");
      },
      s);

    std::format_to(out, "}}");

    return out;
  }
};

#endif
